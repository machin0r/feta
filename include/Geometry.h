#pragma once

#include <iosfwd>


/**
 * @struct Vector3D
 * @brief Represents a vector in 3D space.
 *
 * This structure uses double precision floating-point numbers to represent
 * the x, y, and z components of a vector in three-dimensional space.
 * It can be used to represent directions, offsets, or any other 3D vector quantity.
 */
struct Vector3D{
    double x, y, z;

    Vector3D operator+(const Vector3D& v) const {
        return Vector3D{x + v.x, y + v.y, z + v.z};
    }

    Vector3D operator-(const Vector3D& v) const {
        return Vector3D{x - v.x, y - v.y, z - v.z};
    }
};

/**
 * @brief Overloaded stream insertion operator for Vector3D
 * @param os The output stream to insert into
 * @param vector The Vector3D object to insert
 * @return A reference to the output stream
 *
 * This operator allows Vector3D objects to be easily printed to output streams.
 * The vector will be formatted as "(x, y, z)".
 */
std::ostream& operator<<(std::ostream& os, const Vector3D& vector);

/**
 * @file Geometry.h
 * @brief Defines the basic geometric structures used.
 */

/**
 * @struct Point3D
 * @brief Represents a point in 3D space.
 *
 * This structure uses double precision floating-point numbers to represent
 * the x, y, and z coordinates of a point in three-dimensional space.
 */
struct Point3D{
    double x, y, z;

    Point3D operator+(const Vector3D& v) const {
        return Point3D{x + v.x, y + v.y, z + v.z};
    }

    Point3D operator-(const Vector3D& v) const {
        return Point3D{x - v.x, y - v.y, z - v.z};
    }

    Point3D operator+(const Point3D& p) const {
        return Point3D{x + p.x, y + p.y, z + p.z};
    }

    Point3D operator-(const Point3D& p) const {
        return Point3D{x - p.x, y - p.y, z - p.z};
    }

    Point3D operator*(const double& d) const {
        return Point3D{x*d, y*d, z*d};
    }
};

/**
 * @brief Overloaded stream insertion operator for Point3D
 * @param os The output stream to insert into
 * @param point The Point3D object to insert
 * @return A reference to the output stream
 *
 * This operator allows Point3D objects to be easily printed to output streams.
 * The point will be formatted as "(x, y, z)".
 */
std::ostream& operator<<(std::ostream& os, const Point3D& point);

/**
 * @struct Triangle
 * @brief Represents a triangle in 3D space.
 *
 * This structure defines a triangle using a normal vector and three vertices.
 */
struct Triangle{
    Vector3D normal;
    Point3D vertices[3];
};

/**
 * @brief Overloaded stream insertion operator for Triangle
 * @param os The output stream to insert into
 * @param triangle The Triangle object to insert
 * @return A reference to the output stream
 *
 * This operator allows Triangle objects to be easily printed to output streams.
 * The triangle will be formatted as "Normal: (x, y, z), and vertices: (x, y, z), (x, y, z), (x, y, z)".
 */
std::ostream& operator<<(std::ostream& os, const Triangle& triangle);
