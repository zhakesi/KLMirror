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
precision highp float;
uniform sampler2D u_texture0;
uniform sampler2D u_texture_lut;
in vec2 v_uv;
out vec4 fragColor;
void main()
{
    const float EPS = 0.0001;
    const float pxSize = 512.0;
    vec3 orgColor = texture(u_texture0, v_uv).rgb;
    orgColor = clamp(orgColor, vec3(0.0), vec3(0.95));

    float bValue = (orgColor.b * 255.0) / 4.0;

    //vec2 mulB = clamp(floor(bValue) + vec2(0.0, 1.0), 0.0, 63.0);
    vec2 mulB = clamp(floor(bValue) + vec2(0.0, 1.0), 0.0, 64.0);
    vec2 row = floor(mulB / 8.0 + EPS);
    vec4 row_col = vec4(row, mulB - row * 8.0);
    vec4 lookup = orgColor.ggrr * (63.0/pxSize) + row_col * (64.0/pxSize) + (0.5/pxSize);

    float b1w = bValue - mulB.x;

    vec3 sampled1 = texture(u_texture_lut, lookup.zx).rgb;
    vec3 sampled2 = texture(u_texture_lut, lookup.wy).rgb;

    vec3 color = mix(sampled1, sampled2, b1w);
    fragColor = vec4(color, 1.0);
}
