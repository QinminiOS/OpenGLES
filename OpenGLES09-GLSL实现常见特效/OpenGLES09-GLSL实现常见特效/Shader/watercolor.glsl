//http://tech.it168.com/n/2007-03-29/200703291522292_7.shtml

precision mediump float;

uniform sampler2D image;
uniform sampler2D image1; //noise

varying vec2 vTexcoord;

const float uQuantLevel = 2.0;   // 2-6
const float uWaterPower = 8.0;   // 8-64

const vec2 texSize = vec2(256.0, 256.0);

vec4 quant(vec4 cl, float n)
{
    cl.x = floor(cl.x * 255.0/n)*n/255.0;
    cl.y = floor(cl.y * 255.0/n)*n/255.0;
    cl.z = floor(cl.z * 255.0/n)*n/255.0;
    
    return cl;
}

void main()
{
    vec4 noiseColor = uWaterPower * texture2D(image1, vTexcoord);
    vec2 newUV = vec2(vTexcoord.x + noiseColor.x / texSize.x, vTexcoord.y + noiseColor.y / texSize.y);
    vec4 fColor = texture2D(image, newUV);
    
    vec4 color = quant(fColor, 255.0/pow(2.0, uQuantLevel));
    //vec4 color = vec4(1., 1., .5, 1.);
    gl_FragColor = color;
}
