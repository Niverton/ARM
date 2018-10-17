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
  /*std::vector<Vec3_base<short>> voxels_position;
  std::vector<ushort> voxels_intensity;*/
  std::vector<Value> voxels;
  voxels.reserve(size);
  /*voxels_position.reserve(size);
  voxels_intensity.reserve(size);*/
  for (short k = 0; k < depth; ++k) {
    for (short j = 0; j < line; ++j) {
      for (short i = 0; i < column; ++i) {
        Value v;
        f >> v;
        if (f.fail()) {
          std::cerr << "Error reading file (" << file_name << ")\n";
          std::exit(1);
        }
        /*if (v != 0) {
          voxels_intensity.emplace_back(v);
          Vec3_base<short> p({i, j, k});
          //printf("%d,%d,%d\n", p.x, p.y, p.z);
          voxels_position.emplace_back(p);
        }*/
        voxels.emplace_back(v);
      }
    }
  }
  /*voxels_position.shrink_to_fit();
  voxels_intensity.shrink_to_fit();
  position_array.reserve(voxels_position.size());
  intensity_array.reserve(voxels_position.size());*/

  std::array<Vec3_base<short>, 6> neighbors{Vec3_base<short>{-1, 0, 0},
                                            Vec3_base<short>{ 1, 0, 0},
                                            Vec3_base<short>{ 0,-1, 0},
                                            Vec3_base<short>{ 0, 1, 0},
                                            Vec3_base<short>{ 0, 0,-1},
                                            Vec3_base<short>{ 0, 0, 1}};

  //auto within = [](int a, int val, int b) { return a <= val && val < b; };

  // TODO - DEBUG
  //std::cout << voxels_position.size() << std::endl;
  auto value_at = [](std::vector<Value> vec, short x, short y, short z,
                    short col, short lin)
                      { return vec[x + col * (y + lin * z)]; };

/*
      intensity_array.emplace_back((float)voxels_intensity.at(actual_vox) / (float)max_value);
      Vec3 p = Vec3({-0.5f+(float)i0/(float)column, 
                  -0.5f+(float)j0/(float)line, 
                  -0.5f+(float)k0/(float)depth});
      position_array.emplace_back(p);*/
  // The borders
  for (short i = 0; i < column; ++i) {
    float posI = (float)i / (float)(column - 1) - 0.5f;
    for (short j = 0; j < line; ++j) {
      float posJ = (float)j / (float)(line - 1) - 0.5f;

      short val = value_at(voxels, i, j, 0, column, line);
      if (val != 0) {
        position_array.emplace_back(Vec3({posI, posJ,-0.5f}));
        intensity_array.emplace_back((float)val / (float)max_value);
      }
      val = value_at(voxels, i, j, depth-1, column, line);
      if (val != 0) {
        position_array.emplace_back(Vec3({posI, posJ, 0.5f}));
        intensity_array.emplace_back((float)val / (float)max_value);
      }
    }
    for (short k = 0; k < depth; ++k) {
      float posK = (float)k / (float)(depth - 1) - 0.5f;

      short val = value_at(voxels, i, 0, k, column, line);
      if (val != 0) {
        position_array.emplace_back(Vec3({posI,-0.5f, posK}));
        intensity_array.emplace_back((float)val / (float)max_value);
      }
      val = value_at(voxels, i, line-1, k, column, line);
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

      short val = value_at(voxels, 0, j, k, column, line);
      if (val != 0) {
        position_array.emplace_back(Vec3({-0.5f, posJ, posK}));
        intensity_array.emplace_back((float)val / (float)max_value);
      }
      val = value_at(voxels, column-1, j, k, column, line);
      if (val != 0) {
        position_array.emplace_back(Vec3({ 0.5f, posJ, posK}));
        intensity_array.emplace_back((float)val / (float)max_value);
      }
    }
  }

  // The inside
  for (short k = 1; k < depth - 1; ++k) {
    float posK = (float)k / (float)(depth - 1) - 0.5f;
    for (short j = 1; j < line - 1; ++j) {
      float posJ = (float)j / (float)(line - 1) - 0.5f;
      for (short i = 1; i < column - 1; ++i) {
        float posI = (float)i / (float)(column - 1) - 0.5f;
        short val = value_at(voxels, i, j, k, column, line);
        if (val == 0) {
          continue;
        }
        for (unsigned int neigh = 0; neigh < neighbors.size(); ++neigh) {
          short i_n = i + neighbors[neigh].x;
          short j_n = j + neighbors[neigh].y;
          short k_n = k + neighbors[neigh].z;
          if (val == value_at(voxels, i_n, j_n, k_n, column, line)) {
            //std::cout << "test" << std::endl;
            position_array.emplace_back(Vec3({posI, posJ, posK}));
            intensity_array.emplace_back((float)val / (float)max_value);
          }
        }
      }
    }
  }
  /*for (const Vec3_base & vec : voxels_position) {
    short i0 = vec.x;
    short j0 = vec.y;
    short k0 = vec.z;
    uint res = 0;
    for (uint indice = 0; indice < neighbors.size(); ++indice) {
      if (res != indice) break;
      short i = i0 + neighbors.at(indice).x;
      short j = j0 + neighbors.at(indice).y;
      short k = k0 + neighbors.at(indice).z;
      if (!within(0, i, depth) || !within(0, j, line) || !within(0, k, column)) {
        ++res;
        continue;
      }

      auto it_neigh = std::find(voxels_position.begin(), voxels_position.end(), Vec3_base<short>({i, j, k}));
      if (it_neigh == voxels_position.end()) { continue; }
      int vox = std::distance(voxels_position.begin(), it_neigh);
      if (voxels_intensity.at(actual_vox) == voxels_intensity.at(vox)) {
        ++res;
      }
    }

    if (res != neighbors.size()) {
      intensity_array.emplace_back((float)voxels_intensity.at(actual_vox) / (float)max_value);
      Vec3 p = Vec3({-0.5f+(float)i0/(float)column, 
                  -0.5f+(float)j0/(float)line, 
                  -0.5f+(float)k0/(float)depth});
      position_array.emplace_back(p);
    }
  }*/

  /*position_array.shrink_to_fit();
  intensity_array.shrink_to_fit();*/

  // TODO - DEBUG
  //std::cout << position_array.size() << std::endl;
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
