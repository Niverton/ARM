#include "voxel_mesh.h"
#include <fstream>
#include <iostream>
#include <string>

void VoxelMesh::loadFromPGM3D(const std::string &file_name) {
  std::fstream f{file_name, std::ios::in};
  if (!f.is_open()) {
    std::cerr << "File " << file_name << " not found !\n";
    std::exit(1);
  }
  std::string header;
  std::getline(f, header);
  if (header != "PGM3D") {
    std::cerr << "Invalid file format (" << file_name << ")\n";
    std::exit(1);
  }
  f >> column >> line >> depth;
  f >> max_value;

  unsigned int size = column * line * depth;
  voxel_array.reserve(size);
  for (int i = 0; i < size; ++i) {
    Value v;
    f >> v;
    if (f.fail()) {
      std::cerr << "Error reading file (" << file_name << ")\n";
      std::exit(1);
    }
    voxel_array.emplace_back(v);
  }
}
