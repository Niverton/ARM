#ifndef VOXEL_MESH_H
#define VOXEL_MESH_H

#include "vec.h"
#include <string>
#include <vector>

class VoxelMesh {
public:
  using Value = unsigned short;

  VoxelMesh() = default;
  virtual ~VoxelMesh() = default;

  void loadFromPGM3D(const std::string &file_name);
  std::vector<float> intensity_array;
  std::vector<Vec3> position_array;
  unsigned short column{1}, line{1}, depth{1};

protected:
  float offset[3]{1.0f, 1.0f, 1.0f};
  Value max_value;
};

#endif /* VOXEL_MESH_H */
