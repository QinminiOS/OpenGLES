precision mediump float;

uniform sampler2D image1;
uniform sampler2D image2;

varying vec2 vTexcoord;

void main()
{
// 加法
//    vec4 color1 = texture2D(image1, vTexcoord);
//    vec4 color2 = texture2D(image2, vTexcoord);
//    float alpha = 0.6;
//    gl_FragColor = vec4(vec3(color1*(1.0 - alpha) + color2*alpha), 1.0);
    
//    vec4 color2 = texture2D(image2, vTexcoord);
//    gl_FragColor = vec4(vec3(1.0) - vec3(color2), 1.0);
    
// 减法
//    vec4 color1 = texture2D(image1, vTexcoord);
//    vec4 color2 = texture2D(image2, vTexcoord);
//    gl_FragColor = vec4(vec3(color2 - color1), 1.0);
    
// 乘法
    vec4 color1 = texture2D(image1, vTexcoord);
    vec4 color2 = texture2D(image2, vTexcoord);
    gl_FragColor = vec4(vec3(1.5 * color1 * color2), 1.0);
    
// 除法
//    vec4 color1 = texture2D(image1, vTexcoord);
//    vec4 color2 = texture2D(image2, vTexcoord);
//    gl_FragColor = vec4(vec3(color1/color2), 1.0);

}
