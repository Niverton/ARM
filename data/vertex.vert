#version 330 core

uniform mat4 objectMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 meshBounds;

layout (location = 0) in vec3 vtx_position;
layout (location = 1) in vec3 instance_pos;
layout (location = 2) in float instance_intensity;
out vec3 position;
out float intensity;

void main(void) {
  mat4 MVP = projectionMatrix*viewMatrix*objectMatrix;
  position = (vtx_position/meshBounds + instance_pos.xyz);
  intensity = instance_intensity;
  gl_Position = MVP * vec4(position,1.0);
}