#version 300 es

precision mediump float;

in vec3 outColor;

out vec4 v_color;

void main()
{
    v_color = vec4(outColor, 1.0);
}
