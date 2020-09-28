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
uniform sampler2D u_texY;
uniform sampler2D u_texVU;
in vec2 v_uv;
out vec4 fragColor;
void main()
{
    float rawY = texture(u_texY, v_uv).x;
    vec2 rawUV = texture(u_texVU, v_uv).xy;
    float rawU = rawUV.x - 0.5;
    float rawV = rawUV.y - 0.5;
    float r = rawY + 1.1398 * rawV;
    float g = rawY - 0.3946 * rawU - 0.5806 * rawV;
    float b = rawY + 2.0321 * rawU;
    fragColor = vec4(r, g, b, 1.0);
}