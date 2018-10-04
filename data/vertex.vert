#version 330 core

uniform mat4 objectMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform int column;
uniform int line;
uniform int depth;
//uniform int max_value;
uniform vec3 offset;

in vec3 vtx_position;
out vec3 position;

void main(void) {
  int instanceCol = gl_InstanceID % 64;
  int instanceLin = (gl_InstanceID / 64) % 64;
  int instanceDep = (gl_InstanceID / (64 * 64)) % 64;
  vec3 shift = vec3(offset.x * instanceCol, offset.y * instanceLin, offset.z * instanceDep);

  mat4 MVP = projectionMatrix*viewMatrix*objectMatrix;
  gl_Position = MVP * vec4(vtx_position+shift, 1.0); //viewMatrix*objectMatrix*
  position = vtx_position;
}