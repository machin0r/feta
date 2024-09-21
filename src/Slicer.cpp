#include "Slicer.h"
#include <iostream>
#include <cmath>
#include <vector>

Slicer::Slicer(const STLReader& stlReader, double layerHeight)
    : stlReader(stlReader), layerHeight(layerHeight) {}


void Slicer::sliceModel() {
    const auto& triangles = stlReader.getTriangles();
    double modelHeight = stlReader.getMaximumBoundingBox().z - stlReader.getMinimumBoundingBox().z;
    int numLayers = ceil(modelHeight / layerHeight);

    // Create a new slice layer and check if each triangle is fully in this slice layer, or intersects it
    for (int i = 0; i < numLayers; i++) {
        double layerZ = i * layerHeight;
        Layer currentLayer;
        currentLayer.height = layerZ;

        for (const Triangle& tri : triangles) {
            if (isTriangleInLayer(tri, layerZ, layerHeight)) {
                addProjectedTriangleToLayer(tri, layerZ, currentLayer);
            } else if (doesTriangleIntersectLayer(tri, layerZ)) {
                addIntersectionLinesToLayer(tri, layerZ, currentLayer);
            }
        }

        layers.push_back(currentLayer);
    }
}

const std::vector<Layer>& Slicer::getLayers() const {
    return layers;
}

bool Slicer::isTriangleInLayer(const Triangle& triangle, double layerZ, double thickness) {
    return (triangle.vertices[0].z >= layerZ && triangle.vertices[0].z < layerZ + thickness) &&
           (triangle.vertices[1].z >= layerZ && triangle.vertices[1].z < layerZ + thickness) &&
           (triangle.vertices[2].z >= layerZ && triangle.vertices[2].z < layerZ + thickness);
}

void Slicer::addProjectedTriangleToLayer(const Triangle& triangle, double layerZ, Layer& layer) {
    Point2D v1 = {triangle.vertices[0].x, triangle.vertices[0].y};
    Point2D v2 = {triangle.vertices[1].x, triangle.vertices[1].y};
    Point2D v3 = {triangle.vertices[2].x, triangle.vertices[2].y};

    layer.lines.push_back({v1, v2});
    layer.lines.push_back({v2, v3});
    layer.lines.push_back({v3, v1});
}

bool Slicer::doesTriangleIntersectLayer(const Triangle& triangle, double layerZ) {
    return (triangle.vertices[0].z < layerZ && triangle.vertices[1].z >= layerZ) ||
           (triangle.vertices[1].z < layerZ && triangle.vertices[2].z >= layerZ) ||
           (triangle.vertices[2].z < layerZ && triangle.vertices[0].z >= layerZ);
}

void Slicer::addIntersectionLinesToLayer(const Triangle& triangle, double layerZ, Layer& layer) {
    std::vector<Point2D> intersectionPoints;
    double EPSILON = 1e-6;  // Floating point error

    // Iterate through each triangle edge and see if it is intersected by the layer.
    // If yes, determine how far along the line the intersection is, and add it to our layer.
    for (int i = 0; i < 3; ++i) {
        const Point3D& p1 = triangle.vertices[i];
        const Point3D& p2 = triangle.vertices[(i + 1) % 3];

        // Check if crosses slice plane - one point below, and the other on/above plane
        if ((p1.z < layerZ && p2.z >= layerZ) || (p2.z < layerZ && p1.z >= layerZ)) {
            double dist_along = (layerZ - p1.z) / (p2.z - p1.z);
            Point2D intersection = {
                p1.x + (dist_along * (p2.x - p1.x)),
                p1.y + (dist_along * (p2.y - p1.y))
            };
            intersectionPoints.push_back(intersection);
        }

        if (intersectionPoints.size() == 2) break;  // Can only be two intersection points in this logic 
    }

    // If there are two intersection points, add them to our layer list
    if (intersectionPoints.size() == 2) {
        layer.lines.push_back({intersectionPoints[0], intersectionPoints[1]});
    }
    else if (intersectionPoints.size() != 2) {
        // This shouldn't happen with a triangle
        std::cerr << "Unexpected number of intersection points: " << intersectionPoints.size() << std::endl;
    }
}

