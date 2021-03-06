#version 300 es
uniform mat4 u_2DMatrix;
layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_uv;
out vec2 v_uv;
void main()
{
    v_uv = a_uv;
    gl_Position = u_2DMatrix * vec4(a_Position, 1.0, 1.0);
}

#version 300 es
precision mediump float;
uniform sampler2D u_texture0;
in vec2 v_uv;
out vec4 fragColor;
const float kex = 1.2;
void main()
{
    vec3 color = texture(u_texture0, v_uv).rgb;
    float lum = dot(vec3(0.229, 0.587, 0.114), color);
    float b = (4.0 * kex - 1.0);
    float a = 1.0 - b;
    float f = lum*(a*lum + b);
    fragColor = vec4(f * color, 1.0);
}