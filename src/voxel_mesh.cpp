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
  for (uint i = 0; i < size; ++i) {
    Value v;
    f >> v;
    if (f.fail()) {
      std::cerr << "Error reading file (" << file_name << ")\n";
      std::exit(1);
    }
    voxel_array.emplace_back(v);
  }

  // offset = {1.0f, 1.0f, 1.0f};
}

/*
void VoxelMesh::drawAsCubes(Canvas &canvas) {
  // TODO : Get the Canvas from a cleaner way
  canvas.program->setUniformValue("column", column);
  canvas.program->setUniformValue("line", line);
  canvas.program->setUniformValue("depth", depth);
  canvas.program->setUniformValue("max_value", max_value);
  canvas.program->setUniformValue("offset", offset[0], offset[1], offset[2]);


  //
https://stackoverflow.com/questions/27763851/how-to-use-gldrawelementsinstanced-texture-buffer-objects
  // In shader : gl_InstanceID pour l'instance (glGenVertexArray &
BindVertexArray ? Avec gl_VertexID ?) glDrawElementsInstanced(GL_TRIANGLES,
canvas.faceArray.size() * 3, GL_UNSIGNED_SHORT, nullptr, column * line *
depth);//voxel_array.size();
}
*/
