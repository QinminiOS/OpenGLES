attribute vec3 position;
attribute vec2 texcoord;

uniform mat4 projection;
uniform mat4 modelView;

varying vec2 vTexcoord;

void main()
{
    gl_Position = projection * modelView * vec4(position, 1.0);
    vTexcoord = texcoord;
}
