#pragma once

#include "Geometry.h"
#include "STLReader.h"
#include <vector>
#include <string> 

/**
 * @class STLReader
 * @brief A class for reading and processing STL (STereoLithography) files.
 *
 * This class provides functionality to read STL files, parse the triangles
 * within them, and perform various calculations such as surface area.
 */
class STLReader {
private:
    std::vector<Triangle> triangles;
    double totalSurfaceArea;

    /**
     * @brief Parses a normal vector from a string in the STL file.
     * @param line The string containing the normal vector data.
     * @param normal The Vector3D object to store the parsed normal.
     * @return true if parsing was successful, false otherwise.
     */
    bool parseNormal(const std::string& line, Vector3D& normal);

    /**
     * @brief Parses a vertex from a string in the STL file.
     * @param line The string containing the vertex data.
     * @param vertex The Point3D object to store the parsed vertex.
     * @return true if parsing was successful, false otherwise.
     */
    bool parseVertex(const std::string& line, Point3D& vertex);

     /**
     * @brief Reads a single triangle from the input file stream, comprised of three vertices. Calss validation on each triangle
     * @param file The input file stream.
     * @param triangle The Triangle object to store the read triangle.
     * @return true if reading was successful, false otherwise.
     */
    bool readTriangle(std::ifstream& file, Triangle& triangle);

    /**
     * @brief Calculates the cross product of two edges of a triangle.
     * @param triangle The Triangle object.
     * @return The cross product as a Vector3D.
     */
    Vector3D calculateTriangleCrossProduct(const Triangle& triangle);

    /**
     * @brief Calculates the area of a triangle given its cross product.
     * @param cross The cross product of two edges of the triangle.
     * @return The area of the triangle.
     */
    double calculateTriangleArea(const Vector3D& cross);

    /**
     * @brief Validates a triangle for correctness and updates total surface area.
     * @param triangle The Triangle object to validate.
     * @return true if the triangle is valid, false otherwise.
     */
    bool validateTriangle(const Triangle& triangle);

public:
    /**
     * @brief Default constructor.
     * Initializes totalSurfaceArea to 0.0.
     */
    STLReader() : totalSurfaceArea(0.0) {}

    /**
     * @brief Reads an STL file and processes its contents.
     * @param filename The path to the STL file.
     * @return true if the file was successfully read and processed, false otherwise.
     */
    bool readSTL(const std::string& filename);

    /**
     * @brief Gets the vector of triangles read from the STL file.
     * @return A const reference to the vector of triangles.
     */
    const std::vector<Triangle>& getTriangles() const;

    /**
     * @brief Gets the total surface area of all valid triangles.
     * @return The total surface area.
     */
    double getTotalSurfaceArea() const;
};
