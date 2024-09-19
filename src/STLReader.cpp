#include "Geometry.h"
#include "STLReader.h"
#include <cmath>
#include <fstream>
#include <iostream> 
#include <vector>


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
    }

    file.close();
    return !triangles.empty();
}

const std::vector<Triangle>& STLReader::getTriangles() const {
    return triangles;
}

double STLReader::getTotalSurfaceArea() const {
    return totalSurfaceArea;
}

