<p align="center">
  <img src="img/feta.jpg" width="300" alt="Project Logo">
</p>

# Feta

Feta (Greek for "slice") is a C++ library for reading, validating, and slicing STL files used for 3D printing.

## Building

### Prerequisites

C++17 compatible compiler

CMake 3.10 or higher

### Steps

Clone the repo:

```
git clone https://github.com/machin0r/feta.git
cd feta
```

Create a build directory and navigate to it:

```
mkdir build && cd build
```

Run CMake and build the project:
```
cmake ..
make
```


## Usage

After building the project, you can run the program with the following syntax:

`./feta <stl_file_path> [options]`

### Options

`-s` <value>: Scales the model (applied before setting Z-height)

`-t` <value>: Sets the layer height for slicing (in mm)

`-z` <value>: Sets the Z-height of the model

### Example 

`./feta path/to/your/model.stl -s 1.5 -z 10 -t 0.2`

This command will:

- Read the STL file
- Scale the model by a factor of 1.5
- Set the Z-height to 10mm
- Slice the model with a layer height of 0.2mm

## Output
The program will output:

- Number of triangles read from the STL file
- Total surface area of the model
- Total volume of the model
- Bounding box dimensions
- Number of layers after slicing (if layer height is specified)