#version 330 core

in vec2 vf_TexCoord;

layout(location = 0) out vec4 out_Color;

uniform mat3 u_Rotation;

vec4 shadeSphere(vec3 dir)
{
	vec3 sun_dir = normalize(vec3(0.5));
	if (dot(dir, sun_dir) > 0.97)
		return vec4(1, 1, 0, 1);

	if (dir.y > dir.x*dir.z * 0.5)
		return vec4(mix(vec2(0.15, 0.5), vec2(1, 1), dir.y), 1, 1);
	else if (dir.y < -0.9)
		return vec4(0, 0, 0, 1);
	else
		return vec4(0, 0.5, 0, 1);
}

void main()
{
	vec2 snorm_coord = vf_TexCoord * 2 - 1;
	vec3 dir = vec3(snorm_coord, 1);
	dir = normalize(u_Rotation * dir);
	out_Color = shadeSphere(dir);
}
