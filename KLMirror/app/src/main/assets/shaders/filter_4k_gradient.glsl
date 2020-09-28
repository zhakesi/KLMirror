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
uniform mediump vec2 offset;
in vec2 v_uv;
out vec4 fragColor;
void main()
{
    vec4 c0 = texture(u_texture0, v_uv);
    // [tl tc tr]
    // [ml    mr]
    // [bl bc br]

    float tl = texture(u_texture0, v_uv + vec2(-offset.x, -offset.y)).a;
    float tc = texture(u_texture0, v_uv + vec2(0, -offset.y)).a;
    float tr = texture(u_texture0, v_uv + vec2(offset.x, -offset.y)).a;

    float ml = texture(u_texture0, v_uv + vec2(-offset.x, 0)).a;
    float mr = texture(u_texture0, v_uv + vec2(offset.x, 0)).a;

    float bl = texture(u_texture0, v_uv + vec2(-offset.x, offset.y)).a;
    float bc = texture(u_texture0, v_uv + vec2(0, offset.y)).a;
    float br = texture(u_texture0, v_uv + vec2(offset.x, offset.y)).a;

    vec2 grad = vec2(tr + 2.0 * mr + br - (tl + 2.0 * ml + bl),
                         bl + 2.0 * bc + br - (tl + 2.0 * tc + tr));

    // Computes the luminance's gradient and saves it in the unused alpha channel
    fragColor = vec4(c0.rgb, 1.0 - clamp(length(grad), 0.0, 1.0));
}