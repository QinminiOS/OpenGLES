precision mediump float;

varying vec2 vTexcoord;
uniform sampler2D image;

void main()
{
    vec4 color = texture2D(image, vec2(vTexcoord.x, 1.0 - vTexcoord.y));
    gl_FragColor = color;
}
