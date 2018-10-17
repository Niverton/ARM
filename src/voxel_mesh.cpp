#include "voxel_mesh.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <array>

void VoxelMesh::loadFromPGM3D(const std::string &file_name) {
  //using uint = unsigned int;
  //using ushort = unsigned short;
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
  
  short *matrix_voxel = (short*)malloc(size * sizeof(short));
  auto value_at = [](short* vec, short x, short y, short z,
                    short col, short lin)
                      { return vec[x + col * (y + lin * z)]; };
  
  for (short k = 0; k < depth; ++k) {
    for (short j = 0; j < line; ++j) {
      for (short i = 0; i < column; ++i) {
        Value v;
        f >> v;
        if (f.fail()) {
          std::cerr << "Error reading file (" << file_name << ")\n";
          std::exit(1);
        }
        matrix_voxel[i + column*(j+line*k)] = v;
      }
    }
  }

  std::array<Vec3_base<short>, 6> neighbors{Vec3_base<short>{-1, 0, 0},
                                            Vec3_base<short>{ 1, 0, 0},
                                            Vec3_base<short>{ 0,-1, 0},
                                            Vec3_base<short>{ 0, 1, 0},
                                            Vec3_base<short>{ 0, 0,-1},
                                            Vec3_base<short>{ 0, 0, 1}};

  // The borders of the matrix
  for (short i = 0; i < column; ++i) {
    float posI = (float)i / (float)(column - 1) - 0.5f;
    for (short j = 0; j < line; ++j) {
      float posJ = (float)j / (float)(line - 1) - 0.5f;

      short val = value_at(matrix_voxel, i, j, 0, column, line);
      if (val != 0) {
        position_array.emplace_back(Vec3({posI, posJ,-0.5f}));
        intensity_array.emplace_back((float)val / (float)max_value);
      }
      val = value_at(matrix_voxel, i, j, depth-1, column, line);
      if (val != 0) {
        position_array.emplace_back(Vec3({posI, posJ, 0.5f}));
        intensity_array.emplace_back((float)val / (float)max_value);
      }
    }
    for (short k = 0; k < depth; ++k) {
      float posK = (float)k / (float)(depth - 1) - 0.5f;

      short val = value_at(matrix_voxel, i, 0, k, column, line);
      if (val != 0) {
        position_array.emplace_back(Vec3({posI,-0.5f, posK}));
        intensity_array.emplace_back((float)val / (float)max_value);
      }
      val = value_at(matrix_voxel, i, line-1, k, column, line);
      if (val != 0) {
        position_array.emplace_back(Vec3({posI, 0.5f, posK}));
        intensity_array.emplace_back((float)val / (float)max_value);
      }
    }
  }
  for (short j = 0; j < line; ++j) {
    float posJ = (float)j / (float)(line - 1) - 0.5f;
    for (short k = 0; k < depth; ++k) {
      float posK = (float)k / (float)(depth - 1) - 0.5f;

      short val = value_at(matrix_voxel, 0, j, k, column, line);
      if (val != 0) {
        position_array.emplace_back(Vec3({-0.5f, posJ, posK}));
        intensity_array.emplace_back((float)val / (float)max_value);
      }
      val = value_at(matrix_voxel, column-1, j, k, column, line);
      if (val != 0) {
        position_array.emplace_back(Vec3({ 0.5f, posJ, posK}));
        intensity_array.emplace_back((float)val / (float)max_value);
      }
    }
  }

  // The inside of the matrix
  for (short k = 1; k < depth - 1; ++k) {
    float posK = (float)k / (float)(depth - 1) - 0.5f;
    for (short j = 1; j < line - 1; ++j) {
      float posJ = (float)j / (float)(line - 1) - 0.5f;
      for (short i = 1; i < column - 1; ++i) {
        float posI = (float)i / (float)(column - 1) - 0.5f;
        short val = value_at(matrix_voxel, i, j, k, column, line);
        if (val == 0) {
          continue;
        }
        for (unsigned int neigh = 0; neigh < neighbors.size(); ++neigh) {
          short i_n = i + neighbors[neigh].x;
          short j_n = j + neighbors[neigh].y;
          short k_n = k + neighbors[neigh].z;
          if (val == value_at(matrix_voxel, i_n, j_n, k_n, column, line)) {
            position_array.emplace_back(Vec3({posI, posJ, posK}));
            intensity_array.emplace_back((float)val / (float)max_value);
          }
        }
      }
    }
  }
}
