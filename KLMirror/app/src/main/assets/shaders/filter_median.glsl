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
const vec3 lum_vec = vec3(0.229, 0.587, 0.114);
const vec2 offset1 = vec2(-1.0/640.0, 0.0);
const vec2 offset2 = vec2(1.0/640.0, 0.0);
const vec2 offset3 = vec2(0.0, -1.0/360.0);
const vec2 offset4 = vec2(0.0, 1.0/360.0);
const vec2 offset5 = vec2(-1.0/640.0, -1.0 / 360.0);
const vec2 offset6 = vec2(1.0/640.0, -1.0 / 360.0);
const vec2 offset7 = vec2(-1.0/640.0, 1.0/360.0);
const vec2 offset8 = vec2(1.0/640.0, 1.0/360.0);

void sort2(inout vec3 a0, inout vec3 a1)
{
	vec3 b0 = min(a0, a1);
	vec3 b1 = max(a0, a1);
	a0 = b0;
	a1 = b1;
}

void sort(inout vec3 a0, inout vec3 a1, inout vec3 a2, inout vec3 a3, inout vec3 a4)
{
	sort2(a0, a1);
	sort2(a3, a4);
	sort2(a0, a2);
	sort2(a1, a2);
	sort2(a0, a3);
	sort2(a2, a3);
	sort2(a1, a4);
	sort2(a1, a2);
	sort2(a3, a4);
}

void main()
{
    vec3 color0 = texture(u_texture0, v_uv).rgb;
    vec3 color1 = texture(u_texture0, v_uv + offset1).rgb;
    vec3 color2 = texture(u_texture0, v_uv + offset2).rgb;
    vec3 color3 = texture(u_texture0, v_uv + offset3).rgb;
    vec3 color4 = texture(u_texture0, v_uv + offset4).rgb;

    vec3 color5 = texture(u_texture0, v_uv + offset5).rgb;
    vec3 color6 = texture(u_texture0, v_uv + offset6).rgb;
    vec3 color7 = texture(u_texture0, v_uv + offset7).rgb;
    vec3 color8 = texture(u_texture0, v_uv + offset8).rgb;

    sort(color0, color1, color2, color3, color4);
    sort(color5, color6, color2, color7, color8);
    fragColor = vec4(color2, 1.0);
}