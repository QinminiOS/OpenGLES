precision mediump float;

uniform sampler2D image;

varying vec2 vTexcoord;

void main()
{
    //float depth = gl_FragCoord.z;
    //gl_FragColor = vec4(vec3(depth), 1.0);
    gl_FragColor = texture2D(image, vec2(vTexcoord.x, 1.0 - vTexcoord.y));
}
