#version 330 core

out vec4 out_color;

in vec3 position;
in vec3 color;

void main(void) {
    out_color = vec4(color,1.0);
}
