precision mediump float;

uniform sampler2D image;

varying vec2 vTexcoord;

void main()
{
    float block = 200.0;
    float delta = 1.0/block;
    vec4 maxColor = vec4(1.0);
    
    for (int i = -1; i <= 1 ; i++) {
        for (int j = -1; j <= 1; j++) {
            float x = vTexcoord.x + float(i) * delta;
            float y = vTexcoord.y + float(i) * delta;
            maxColor = min(texture2D(image, vec2(x, y)), maxColor);
        }
    }
    
    gl_FragColor = maxColor;
}
