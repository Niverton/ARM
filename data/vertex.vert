#version 330 core

uniform mat4 objectMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

layout (location = 0) in vec3 vtx_position;
layout (location = 1) in vec3 shift;
out vec3 position;

void main(void) {
  mat4 MVP = projectionMatrix*viewMatrix*objectMatrix;
  position = vtx_position + shift;
  gl_Position = MVP * vec4(vtx_position + shift,1.0);
}