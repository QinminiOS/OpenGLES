precision mediump float;

varying vec2 v_texcoord;

uniform sampler2D image0;
uniform sampler2D image1;
uniform sampler2D image2;

void main()
{
    highp float y = texture2D(image0, v_texcoord).r;
    highp float u = texture2D(image1, v_texcoord).r - 0.5;
    highp float v = texture2D(image2, v_texcoord).r - 0.5;
    
    highp float r = y + 0.000     + 1.402 * v;
    highp float g = y - 0.344 * u - 0.714 * v;
    highp float b = y + 1.772 * u;
    
    gl_FragColor = vec4(r, g, b, 1.0);
}
