#version 300 es
uniform mat4 u_2DMatrix;
uniform mat2 u_flipMatrix;
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
uniform sampler2D u_texSRC;
uniform sampler2D u_texHoriz;
uniform vec2 offset1;
uniform vec2 offset2;
in vec2 v_uv;
out vec4 fragColor;
vec3 weight = vec3(0.2270270270, 0.3162162162, 0.0702702703 );
void main()
{
    vec3 src = texture(u_texSRC, v_uv).rgb;
    vec3 blur = texture(u_texHoriz, v_uv).rgb * weight[0];
    blur = blur + texture(u_texHoriz, v_uv + offset1).rgb * weight[1];
    blur = blur + texture(u_texHoriz, v_uv - offset1).rgb * weight[1];
    blur = blur + texture(u_texHoriz, v_uv + offset2).rgb * weight[2];
    blur = blur + texture(u_texHoriz, v_uv - offset2).rgb * weight[2];
    fragColor = vec4(blur.rgb, 1.0);
}