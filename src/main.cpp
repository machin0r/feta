#include <iostream>
#include <string>
#include <optional>
#include "STLReader.h"

void printUsage(const char* programName) {
    std::cerr << "Usage: " << programName << " <stl_file_path> [options]" << std::endl;
    std::cerr << "Options:" << std::endl;
    std::cerr << "  -s <value>    Scales the model, applied before -z" << std::endl;
    std::cerr << "  -z <value>    Set Z-height of the model" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    std::string filename = argv[1];
    STLReader reader;

    std::optional<float> zHeight;
    std::optional<float> scaleFactor;


    // Parse command-line arguments
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-z" && i + 1 < argc) {
            zHeight = std::stof(argv[++i]);
        }
        if (arg == "-s" && i + 1 < argc) {
            scaleFactor = std::stof(argv[++i]);
        }
    }

    if (reader.readSTL(filename)) {
        std::cout << "Successfully read " << reader.getTriangles().size() << " triangles." << std::endl;
    } else {
        std::cerr << "Failed to read STL file." << std::endl;
    }

    if (scaleFactor.has_value()) {
        reader.scaleModel(scaleFactor.value());
        std::cout << "Model scaled by a factor of " << scaleFactor.value() << std::endl;
    }

    if (zHeight.has_value()) {
        reader.setZHeight(zHeight.value());
        std::cout << "Set Z height to " << zHeight.value() << std::endl;
    }

    std::cout << "The total surface area of the part is " << reader.getTotalSurfaceArea() << " mm^2." << std::endl;
    std::cout << "The total volume of the part is " << reader.getVolume() << " mm^3." << std::endl;
    std::cout << "The model bounding box is: Minimum: " << reader.getMinimumBoundingBox() << " and Maximum: " << reader.getMaximumBoundingBox() << std::endl;
    return 0;
}