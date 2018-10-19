#version 330 core

uniform mat4 objectMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

layout (location = 0) in vec3 vtx_position;
out vec3 position;
out vec3 color;

void main(void) {
  mat4 MVP = projectionMatrix*viewMatrix*objectMatrix;
  color = vec3(1,1,1);
  gl_Position = MVP * vec4(vtx_position,1.0);
}
