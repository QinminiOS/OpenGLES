precision mediump float;

varying vec2 v_texcoord;

uniform sampler2D image0;

void main()
{
    // bgra
    // rgba
    vec4 color = texture2D(image0, v_texcoord);
    gl_FragColor = vec4(color.bgr, 1.0);
}
