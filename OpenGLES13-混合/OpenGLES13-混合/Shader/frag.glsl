precision mediump float;

uniform sampler2D image;
uniform sampler2D image1;

varying vec2 vTexcoord;

void main()
{
    //gl_FragColor = texture2D(image, vTexcoord);
    
    vec4 des = vec4(1.0, 0.0, 0.0, 1.0);
    vec4 src = texture2D(image1, vTexcoord);
    vec4 bld;
    if (vTexcoord.x <= 0.5) {
        bld = vec4(vec3(des), src.a);
    }else {
        bld = src;
    }
    
    gl_FragColor = bld * bld.a + vec4(1.0) * (1.0 - bld.a);
}
