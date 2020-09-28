#version 300 es
uniform mat4 u_2DMatrix;
layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_uv;
out vec2 v_uv;
void main()
{
    v_uv = vec2(a_uv.x, 1.0 - a_uv.y);
    gl_Position = u_2DMatrix * vec4(a_Position, 1.0, 1.0);
}

#version 300 es
precision mediump float;
uniform sampler2D u_texture0;
in vec2 v_uv;
out vec4 fragColor;
void main()
{
    vec3 color = texture(u_texture0, v_uv).rgb;
    float lum = dot(vec3(0.229, 0.587, 0.114), color);
    fragColor = vec4(lum, lum, lum,1.0);
}