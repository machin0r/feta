#include "Geometry.h"
#include "STLReader.h"
#include <cmath>
#include <fstream>
#include <iostream> 
#include <vector>

STLReader::STLReader() 
    : totalSurfaceArea(0.0),
      volume(0.0),
      volumeCalculated(false),
      minBound{std::numeric_limits<double>::max(),
               std::numeric_limits<double>::max(),
               std::numeric_limits<double>::max()},
      maxBound{std::numeric_limits<double>::lowest(),
               std::numeric_limits<double>::lowest(),
               std::numeric_limits<double>::lowest()},
      appliedTranslation{0,0,0}
{}

bool STLReader::parseNormal(const std::string& line, Vector3D& normal){
    return sscanf(line.c_str(), " facet normal %lf %lf %lf", 
        &normal.x, 
        &normal.y, 
        &normal.z) == 3;
}

bool STLReader::parseVertex(const std::string& line, Point3D& vertex) {
    return sscanf(line.c_str(), " vertex %lf %lf %lf",
                &vertex.x, &vertex.y, &vertex.z) == 3;
}

bool STLReader::readTriangle(std::ifstream& file, Triangle& triangle) {
    std::string line; 

    if (!std::getline(file, line) || line.find("facet normal") == std::string::npos) {
        return false;
    }

    if (!parseNormal(line, triangle.normal)) {
        return false;
    }

    // skip outerline
    if (!std::getline(file, line) || line.find("outer loop") == std::string::npos) {
        return false;
    }

    for (int i = 0; i < 3; ++i){
        if (!std::getline(file, line) || !parseVertex(line, triangle.vertices[i])) {
            return false;
        }
    }
    
    // Skip endloop and endfacet lines we know will be there
    if (!std::getline(file, line) || line.find("endloop") == std::string::npos) {
        return false;
    }
    if (!std::getline(file, line) || line.find("endfacet") == std::string::npos) {
        return false;
    }

    if (!validateTriangle(triangle)) {
    // Handle invalid triangle (log, skip, or throw exception)
    return false;
}

    return true;
}

bool STLReader::validateTriangle(const Triangle& triangle) {
    // floating point comparison epsilon
    const double epsilon = 1e-6;

    Vector3D cross = calculateTriangleCrossProduct(triangle);

    double area = calculateTriangleArea(cross);

    totalSurfaceArea += area;  // Add to total surface area

    if (area < epsilon) {
        return false;  // Degenerate triangle
    }
    
    // Check if normal is a unit vector
    double normal_length = sqrt(triangle.normal.x*triangle.normal.x + 
                                triangle.normal.y*triangle.normal.y + 
                                triangle.normal.z*triangle.normal.z);
    if (std::abs(normal_length - 1.0) > epsilon) {
        return false;  // Normal is not a unit vector
    }

    // Check if normal is perpendicular to triangle plane
    Vector3D calculated_normal = {
        cross.x / (2 * area),
        cross.y / (2 * area),
        cross.z / (2 * area)
    };
    
    double dot_product = calculated_normal.x * triangle.normal.x +
                        calculated_normal.y * triangle.normal.y +
                        calculated_normal.z * triangle.normal.z;
    
    if (std::abs(std::abs(dot_product) - 1) > epsilon) {
        return false;  // Normal is not perpendicular to triangle plane
    }

    return true;
}

Vector3D STLReader::calculateTriangleCrossProduct(const Triangle& triangle) {
    Vector3D edge1 = {
        triangle.vertices[1].x - triangle.vertices[0].x,
        triangle.vertices[1].y - triangle.vertices[0].y,
        triangle.vertices[1].z - triangle.vertices[0].z
    };
    Vector3D edge2 = {
        triangle.vertices[2].x - triangle.vertices[0].x,
        triangle.vertices[2].y - triangle.vertices[0].y,
        triangle.vertices[2].z - triangle.vertices[0].z
    };
    
    Vector3D cross = {
        edge1.y * edge2.z - edge1.z * edge2.y,
        edge1.z * edge2.x - edge1.x * edge2.z,
        edge1.x * edge2.y - edge1.y * edge2.x
    };

    return cross;
}

double STLReader::calculateTriangleArea(const Vector3D& cross) {
    return 0.5 * sqrt(cross.x*cross.x + cross.y*cross.y + cross.z*cross.z);
}

void STLReader::updateBoundingBox(const Triangle& triangle) {
    for (const auto& vertex : triangle.vertices) {
            minBound.x = std::min(minBound.x, vertex.x);
            minBound.y = std::min(minBound.y, vertex.y);
            minBound.z = std::min(minBound.z, vertex.z);
            maxBound.x = std::max(maxBound.x, vertex.x);
            maxBound.y = std::max(maxBound.y, vertex.y);
            maxBound.z = std::max(maxBound.z, vertex.z);
        }
}

void STLReader::translateVertex(Point3D& vertex, Vector3D translation) {
    vertex = vertex + translation;
}

bool STLReader::readSTL(const std::string& filename){
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return false;
    }

    totalSurfaceArea = 0.0;

    std::string line; 
    std::getline(file, line); // Skip first line (solid name)

    Triangle triangle;
    while (readTriangle(file, triangle)) {
        triangles.push_back(triangle);
        updateBoundingBox(triangle);
    }

    file.close();

    volume = calculateVolume();
    
    return !triangles.empty();
}

void STLReader::updateModelStats() {
    totalSurfaceArea = 0.0;
    for (const auto& triangle : triangles) {
            if (validateTriangle(triangle)) {
                Vector3D cross = calculateTriangleCrossProduct(triangle);
                totalSurfaceArea += calculateTriangleArea(cross);
                updateBoundingBox(triangle);
            }
    }
    volume = calculateVolume();
}

const std::vector<Triangle>& STLReader::getTriangles() const {
    return triangles;
}

double STLReader::calculateVolume() {
    if (!volumeCalculated) {
        volume = 0.0;
        // Use Signed Tetrahedron Volume for volume of each triangle to origin
        for (const auto& triangle : triangles) {
            volume += (triangle.vertices[0].x + triangle.vertices[1].x + triangle.vertices[2].x) *
                      (triangle.vertices[1].y - triangle.vertices[0].y) *
                      (triangle.vertices[2].z - triangle.vertices[0].z);
        }
        volume = std::abs(volume) / 6.0;
        volumeCalculated = true;
    }
    return volume;
}

double STLReader::getVolume() const {
    return volume;
}

double STLReader::getTotalSurfaceArea() const {
    return totalSurfaceArea;
}

Point3D STLReader::getMinimumBoundingBox() const {
    return minBound;
}

Point3D STLReader::getMaximumBoundingBox() const {
    return maxBound;
}

void STLReader::translateModel(Vector3D translation) {
    for (auto& triangle : triangles) {
            for (auto& vertex : triangle.vertices) {
                translateVertex(vertex, translation);
            }
        }
    minBound = minBound + translation;
    maxBound = maxBound + translation;

    appliedTranslation = appliedTranslation + translation;
}

void STLReader::setZHeight(double desiredZHeight) {
    double zTranslation = desiredZHeight - minBound.z;
    translateModel(Vector3D{0, 0, zTranslation});
}