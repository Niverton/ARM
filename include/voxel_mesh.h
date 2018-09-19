#ifndef VOXEL_MESH_H
#define VOXEL_MESH_H

#include <string>
#include <vector>

class VoxelMesh {
public:
  using Value = unsigned short;

  VoxelMesh() = default;
  virtual ~VoxelMesh() = default;

  void loadFromPGM3D(const std::string &file_name);

protected:
  unsigned int column{0}, line{0}, depth{0};
  Value max_value;
  std::vector<Value> voxel_array;
};

#endif /* VOXEL_MESH_H */
