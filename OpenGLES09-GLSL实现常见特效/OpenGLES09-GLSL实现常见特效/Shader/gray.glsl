//灰度图
precision highp float;
uniform sampler2D image;
varying vec2 vTexcoord;
const highp vec3 W = vec3(0.2125, 0.7154, 0.0721);

void main()
{
    lowp vec4 textureColor = texture2D(image, vTexcoord);
    float luminance = dot(textureColor.rgb, W);
    
    gl_FragColor = vec4(vec3(luminance), textureColor.a);
}
