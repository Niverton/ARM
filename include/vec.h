#ifndef CANVAS_H
#define CANVAS_H

template <typename T>
struct Vec3_base {
  T x=0, y=0, z=0;
};
template <typename T>
struct Vec4_base {
  T x=0, y=0, z=0, w=0;
};

using Vec3 = Vec3_base<float>;
using Vec4 = Vec4_base<float>;

#endif