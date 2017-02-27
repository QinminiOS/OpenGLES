precision mediump float;

const float time = 100.0;
const vec2 resolution = vec2(414.0, 736.0);

void main()
{
    vec2 pos = -1. + 2.*gl_FragCoord.xy / resolution.xy;
    pos *= vec2(resolution.x / resolution.y, 1.) * 3.;
    
//    // 火苗的抖动
//    if(pos.y>-2.*4.2)
//    {
//        for(float baud = 1.; baud < 9.; baud += 1.)
//        {
//            pos.y += 0.2*sin(4.20*time/(1.+baud))/(1.+baud);
//            pos.x += 0.1*cos(pos.y/4.20+2.40*time/(1.+baud))/(1.+baud);
//        }
//        pos.y += 0.04*fract(sin(time*60.));
//    }
    
    // 火苗外焰
    vec3 color = vec3(0.,0.,0.);
    float p =.004;
    float y = -pow(abs(pos.x), 4.2)/p;   // 外焰的形状，注意pos.x负数会被截断
    float dir = abs(pos.y - y)*sin(.3);  // 外焰的大小（扩大渐变区域）
    //float dir = abs(pos.y - y)*(0.01*sin(time)+0.07);
    if(dir < 0.7)
    {
        color.rg += smoothstep(0.,1.,.75-dir);   // 外焰颜色渐变
        color.g /=2.4;                           // 减点绿
    }
    color *= (0.2 + abs(pos.y/4.2 + 4.2)/4.2);  // 增加对比度
    color += pow(color.r, 1.1);                 // 加点红
    color *= cos(-0.5+pos.y*0.4);               // 隐藏底部的颜色
    
    // 火苗内焰
    pos.y += 1.5;
    vec3 dolor = vec3(0.,0.,0.0);
    y = -pow(abs(pos.x), 4.2)/(4.2*p)*4.2;   // 内焰的形状，注意和外焰的次幂，越接近越不容易穿帮
    dir = abs(pos.y - y)*sin(1.1);           // 内焰的大小（扩大渐变区域）
    if(dir < 0.7)
    {
        dolor.bg += smoothstep(0., 1., .75-dir);// 内焰颜色渐变
        dolor.g /=2.4;
    }
    dolor *= (0.2 + abs((pos.y/4.2+4.2))/4.2);
    dolor += pow(color.b,1.1);                 // 加点蓝
    dolor *= cos(-0.6+pos.y*0.4);
    //dolor.rgb -= pow(length(dolor)/16., 0.5);
    
    color = (color+dolor)/2.;
    gl_FragColor = vec4(vec3(color) , 1.0 );
    
}
