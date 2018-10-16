#include "voxel_mesh.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

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
  intensity_array.reserve(size);
  for (uint i = 0; i < depth; ++i) {
    for (uint j = 0; j < line; ++j) {
      for (uint k = 0; k < column; ++k) {
        Value v;
        f >> v;
        if (f.fail()) {
          std::cerr << "Error reading file (" << file_name << ")\n";
          std::exit(1);
        }
        if (v != 0) {
          intensity_array.emplace_back((float)v / (float)max_value);
          Vec3 p = Vec3({-0.5f+(float)k/(float)column, 
                      -0.5f+(float)j/(float)line, 
                      -0.5f+(float)i/(float)depth});

          position_array.emplace_back(p);

          //printf("%f,%f,%f\n", p.x, p.y, p.z);
        }
      }
    }
  }

  /*std::vector<Vec3> neighbors;
  neighbors.emplace_back(Vec3{-1, 0, 0});
  neighbors.emplace_back(Vec3{ 1, 0, 0});
  neighbors.emplace_back(Vec3{ 0,-1, 0});
  neighbors.emplace_back(Vec3{ 0, 1, 0});
  neighbors.emplace_back(Vec3{ 0, 0,-1});
  neighbors.emplace_back(Vec3{ 0, 0, 1});

  auto within = [](int a, int val, int b) { return a <= val && val < b; };

  auto are_equals = [](float a, float b) { return std::abs(a - b) < 0.00001f; };

  std::vector<int> to_delete;

  std::cout << position_array.size() << std::endl;
  for (int actual_vox = 0; actual_vox < position_array.size(); ++actual_vox) {
    int i0 = position_array.at(actual_vox).x;
    int j0 = position_array.at(actual_vox).y;
    int k0 = position_array.at(actual_vox).z;
    uint res = 0;
    for (uint indice = 0; indice < neighbors.size(); ++indice) {
//std::cout << "res:" << res << " ind:" << indice << std::endl;
      if (res != indice) break;
      int i = i0 + neighbors.at(indice).x;
      int j = j0 + neighbors.at(indice).y;
      int k = k0 + neighbors.at(indice).z;
      if (!within(0, i, depth) || !within(0, j, line) || !within(0, k, column)) {
        ++res;
        continue; // res++;
      }

      for (uint vox = 0; vox < position_array.size(); ++vox) {
        // UTILISER FIND() PLUTOT http://www.cplusplus.com/reference/algorithm/find/
        if (are_equals(position_array.at(vox).x, (float)i)
          && are_equals(position_array.at(vox).y, (float)j)
          && are_equals(position_array.at(vox).z, (float)k))
        {
          if (are_equals(intensity_array.at(actual_vox), intensity_array.at(vox))) {
            ++res;
          }
          break;
        }
      }
    }
//std::cout << res << std::endl;
    if (res == neighbors.size()) {
//std::cout << "test" << std::endl;
      to_delete.emplace_back(actual_vox);
    }
  }

  for(int i = to_delete.size() - 1; i >= 0; --i) {
    position_array.erase(position_array.begin() + to_delete.at(i));
    intensity_array.erase(intensity_array.begin() + to_delete.at(i));
  }
  std::cout << position_array.size() << std::endl;*/
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
