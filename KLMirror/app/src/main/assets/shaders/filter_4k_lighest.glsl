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

vec4 Largest(vec4 mc, vec4 lightest, vec4 a, vec4 b, vec4 c)
{
    vec4 abc = mix(mc, 0.333 * (a + b + c), 0.3);
    return abc.a > lightest.a ? abc : lightest;
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

    vec4 lightest = mc;

    // Kernel 0 and 4
    if (MinA(tl, tc, tr) > MaxA(mc, br, bc, bl))
        lightest = Largest(mc, lightest, tl, tc, tr);
    else if (MinA(br, bc, bl) > MaxA(mc, tl, tc, tr))
        lightest = Largest(mc, lightest, br, bc, bl);

    // Kernel 1 and 5
    if (MinA(mr, tc, tr) > MaxA(mc, ml, bc))
        lightest = Largest(mc, lightest, mr, tc, tr);
    else if (MinA(bl, ml, bc) > MaxA(mc, mr, tc))
        lightest = Largest(mc, lightest, bl, ml, bc);

    // Kernel 2 and 6
    if (MinA(mr, br, tr) > MaxA(mc, ml, tl, bl))
        lightest = Largest(mc, lightest, mr, br, tr);
    else if (MinA(ml, tl, bl) > MaxA(mc, mr, br, tr))
        lightest = Largest(mc, lightest, ml, tl, bl);

    //Kernel 3 and 7
    if (MinA(mr, br, bc) > MaxA(mc, ml, tc))
        lightest = Largest(mc, lightest, mr, br, bc);
    else if (MinA(tc, ml, tl) > MaxA(mc, mr, bc))
        lightest = Largest(mc, lightest, tc, ml, tl);

    fragColor = lightest;
}
