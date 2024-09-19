#include <iostream>
#include <string>
#include "STLReader.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <stl_file_path>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    STLReader reader;

    if (reader.readSTL(filename)) {
        std::cout << "Successfully read " << reader.getTriangles().size() << " triangles." << std::endl;
        std::cout << "The total surface area of the part is " << reader.getTotalSurfaceArea() << " mm^2." << std::endl;
    } else {
        std::cerr << "Failed to read STL file." << std::endl;
    }
    return 0;
}