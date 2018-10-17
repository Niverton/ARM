#include "voxel_mesh.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

void VoxelMesh::loadFromPGM3D(const std::string &file_name) {
  using uint = unsigned int;
  using ushort = unsigned short;
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
  std::vector<Vec3_base<ushort>> voxels_position;
  std::vector<ushort> voxels_intensity;
  for (uint k = 0; k < depth; ++k) {
    for (uint j = 0; j < line; ++j) {
      for (uint i = 0; i < column; ++i) {
        Value v;
        f >> v;
        if (f.fail()) {
          std::cerr << "Error reading file (" << file_name << ")\n";
          std::exit(1);
        }
        if (v != 0) {
          voxels_intensity.emplace_back(v);
          Vec3_base<ushort> p = Vec3_base<ushort>({i, j, k});
          //printf("%d,%d,%d\n", p.x, p.y, p.z);
          voxels_position.emplace_back(p);
        }
      }
    }
  }

  std::vector<Vec3> neighbors;
  neighbors.emplace_back(Vec3{-1, 0, 0});
  neighbors.emplace_back(Vec3{ 1, 0, 0});
  neighbors.emplace_back(Vec3{ 0,-1, 0});
  neighbors.emplace_back(Vec3{ 0, 1, 0});
  neighbors.emplace_back(Vec3{ 0, 0,-1});
  neighbors.emplace_back(Vec3{ 0, 0, 1});

  auto within = [](int a, int val, int b) { return a <= val && val < b; };

  std::vector<int> to_delete;
  std::cout << voxels_position.size() << std::endl;
  for (int actual_vox = 0; actual_vox < voxels_position.size(); ++actual_vox) {
    int i0 = voxels_position.at(actual_vox).x;
    int j0 = voxels_position.at(actual_vox).y;
    int k0 = voxels_position.at(actual_vox).z;
    uint res = 0;
    for (uint indice = 0; indice < neighbors.size(); ++indice) {
      if (res != indice) break;
      int i = i0 + neighbors.at(indice).x;
      int j = j0 + neighbors.at(indice).y;
      int k = k0 + neighbors.at(indice).z;
      if (!within(0, i, depth) || !within(0, j, line) || !within(0, k, column)) {
        ++res;
        continue;
      }

      for (uint vox = 0; vox < voxels_position.size(); ++vox) {
        // UTILISER FIND() PLUTOT http://www.cplusplus.com/reference/algorithm/find/
        if (voxels_position.at(vox).x == i
          && voxels_position.at(vox).y == j
          && voxels_position.at(vox).z == k)
        {
          if (voxels_intensity.at(actual_vox) == voxels_intensity.at(vox)) {
            ++res;
          }
          break;
        }
      }
    }

    if (res != neighbors.size()) {
      intensity_array.emplace_back((float)voxels_intensity.at(actual_vox) / (float)max_value);
      Vec3 p = Vec3({-0.5f+(float)i0/(float)column, 
                  -0.5f+(float)j0/(float)line, 
                  -0.5f+(float)k0/(float)depth});
      position_array.emplace_back(p);
    }
  }
  std::cout << position_array.size() << std::endl;
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
