#pragma once

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
};

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
};

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