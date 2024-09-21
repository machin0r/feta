#include "Geometry.h"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Point2D& point) {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Point3D& point) {
    os << "(" << point.x << ", " << point.y << ", " << point.z << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Vector3D& vector) {
    os << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Triangle& triangle) {
    os << "Normal:" << triangle.normal << ", and vertices:" << triangle.vertices[0] << ", " << triangle.vertices[1] << ", " << triangle.vertices[2] << ")";
    return os;
}
