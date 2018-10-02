#version 330 core

uniform mat4 objectMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

in vec3 vtx_position;
out vec3 position;

void main(void) {
  mat4 MVP = projectionMatrix*viewMatrix*objectMatrix;
  gl_Position = MVP * vec4(vtx_position, 1.0); //viewMatrix*objectMatrix*
  position = vtx_position;
}