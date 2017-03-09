attribute vec3 position;
attribute vec3 offset;
attribute vec2 texcoord;

varying vec2 vTexcoord;

void main()
{
    gl_Position = vec4(position+offset, 1.0);
    vTexcoord = texcoord;
}
