#version 330 core

out vec4 out_color;

in vec3 position;

void main(void) {
    out_color = vec4(position + 0.5,1);
}