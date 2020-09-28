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

float MinA(vec4 a, vec4 b, vec4 c)
{
    return min(min(a.a, b.a), c.a);
}

float MaxA(vec4 a, vec4 b, vec4 c)
{
    return max(max(a.a, b.a), c.a);
}

float MinA(vec4 a, vec4 b, vec4 c, vec4 d)
{
    return min(min(min(a.a, b.a), c.a), d.a);
}

float MaxA(vec4 a, vec4 b, vec4 c, vec4 d)
{
    return max(max(max(a.a, b.a), c.a), d.a);
}

vec4 Average(vec4 mc, vec4 a, vec4 b, vec4 c)
{
    return vec4(mix(mc, 0.333 * (a + b + c), 1.0).rgb, 0.9);
}

void main()
{
    vec4 tl = texture(u_texture0, v_uv + vec2(-offset.x, -offset.y));
    vec4 tc = texture(u_texture0, v_uv + vec2(0, -offset.y));
    vec4 tr = texture(u_texture0, v_uv + vec2(offset.x, -offset.y));

    vec4 ml = texture(u_texture0, v_uv + vec2(-offset.x, 0));
    vec4 mc = texture(u_texture0, v_uv + vec2(0, 0));
    vec4 mr = texture(u_texture0, v_uv + vec2(offset.x, 0));

    vec4 bl = texture(u_texture0, v_uv + vec2(-offset.x, offset.y));
    vec4 bc = texture(u_texture0, v_uv + vec2(0, offset.y));
    vec4 br = texture(u_texture0, v_uv + vec2(offset.x, offset.y));


    // Kernel 0 and 4
    if (MinA(tl, tc, tr) > MaxA(mc, br, bc, bl)) {
        fragColor = Average(mc, tl, tc, tr);
        return;
    }
    if (MinA(br, bc, bl) > MaxA(mc, tl, tc, tr)) {
        fragColor = Average(mc, br, bc, bl);
        return;
    }

    // Kernel 1 and 5
    if (MinA(mr, tc, tr) > MaxA(mc, ml, bc)) {
        fragColor = Average(mc, mr, tc, tr);
        return;
    }
    if (MinA(bl, ml, bc) > MaxA(mc, mr, tc)) {
        fragColor = Average(mc, bl, ml, bc);
        return;
    }

    // Kernel 2 and 6
    if (MinA(mr, br, tr) > MaxA(mc, ml, tl, bl)) {
        fragColor = Average(mc, mr, br, tr);
        return;
    }
    if (MinA(ml, tl, bl) > MaxA(mc, mr, br, tr)) {
        fragColor = Average(mc, ml, tl, bl);
        return;
    }

    // Kernel 3 and 7
    if (MinA(mr, br, bc) > MaxA(mc, ml, tc )) {
        fragColor = Average(mc, mr, br, bc);
        return;
    }
    if (MinA(tc, ml, tl) > MaxA(mc, mr, bc )) {
        fragColor = Average(mc, tc, ml, tl);
        return;
    }
    // mc.rgb = pow(mc.rgb, vec3(0.8, 0.8, 0.8));
    fragColor = vec4(mc.rgb, 1.0);
}