#pragma once

#include "Geometry.h"
#include "STLReader.h"
#include <vector>

/**
 * @class Slicer
 * @brief A class for slicing a series of triangles with an intersecting Z-plane.
 *
 * This class provides functionality to move through a triangle collection and
 * determine if a triangle is fully captured by a slice layer, or intersects it.
 * If fully captured, the triangle is projected onto the slice layer. 
 * If intersected, the intersection line is added.
 */
class Slicer {
public:
/**
     * @brief Constructor for the Slicer class.
     * @param stlReader Reference to an STLReader object containing the 3D model data.
     * @param layerHeight The height of each slice layer.
     */
    Slicer(const STLReader& stlReader, double layerHeight);

    /**
     * @brief Performs the slicing operation on the 3D model.
     */
    void sliceModel();

    /**
     * @brief Gets the slice layers.
     * @return The layers.
     */
    const std::vector<Layer>& getLayers() const;

private:

    struct TriangleZRange {
        const Triangle* triangle;
        double minZ;
        double maxZ;

        TriangleZRange(const Triangle* t);
    };


    const STLReader& stlReader; ///< Reference to the STLReader object containing the 3D model data.
    double layerHeight; ///< The height of each slice layer.
    std::vector<Layer> layers; ///< Vector to store the resulting slice layers.
    std::vector<TriangleZRange> triangleRanges; ///< Vector to store the sorted triangles

    /**
     * @brief Prepares the triangles by sorting them by Z-height
     */
    void prepareTriangles();

    /**
     * @brief Checks to see if a triangle is fully contained by a slice layer.
     * @param triangle The Triangle object to check.
     * @param layerZ The current layer Z-height.
     * @param thickness The thickness of a layer.
     * @return true if the triangle is fully captured by a layer.
     */
    bool isTriangleInLayer(const Triangle& triangle, double layerZ, double thickness);

    /**
     * @brief Projects a triangle onto the slice layer and adds it to the Layer
     * @param triangle The Triangle object to add.
     * @param layerZ The current layer Z-height.
     * @param layer The layer to add the triangle to.
     */
    void addProjectedTriangleToLayer(const Triangle& triangle, double layerZ, Layer& layer);

    /**
     * @brief Checks to see if a triangle intersects a slice layer.
     * @param triangle The Triangle object to check.
     * @param layerZ The current layer Z-height.
     * @return true if the triangle is intersected by a layer.
     */
    bool doesTriangleIntersectLayer(const Triangle& triangle, double layerZ);

    /**
     * @brief Adds the intersection line of the triangle along the slice plane to the Layer.
     * @param triangle The Triangle object to intersect and add.
     * @param layerZ The current layer Z-height.
     * @param layer The layer to add the triangle intersection points to.
     */
    void addIntersectionLinesToLayer(const Triangle& triangle, double layerZ, Layer& layer);
};
