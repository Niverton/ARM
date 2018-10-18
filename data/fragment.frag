#version 330 core

out vec4 out_color;

in vec3 position;
in float intensity;

//http://otherthings.com/blog/2015/03/fun-with-pseudocolor/
vec4 pseudo3(float val, float alpha) {

    float reps = 20.0;
    float pi = 3.14159256;
    float bright = val;
    float con = 0.25 - 0.20 * cos(val * pi * 2.0);
    float sat = 0.66 - 0.25 * cos(val * pi * 2.0);

    return vec4(sin(val*pi*reps)*con+bright,
                sin(val*pi*reps - pi*0.663 * sat)*con+bright,
                sin(val*pi*reps - pi*1.333 * sat)*con+bright,
                1.0)*alpha;
}

void main(void) {
    gl_FragDepth = intensity;
    out_color = pseudo3(intensity, 0.07*(1-intensity));
}
