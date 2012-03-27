#version 330 core

in vec2 vf_TexCoord;

layout(location = 0) out vec4 out_Color;

uniform mat3 u_Rotation;

uniform vec3 u_SHcoef_0p0;
uniform vec3 u_SHcoef_1n1;
uniform vec3 u_SHcoef_1p0;
uniform vec3 u_SHcoef_1p1;
uniform vec3 u_SHcoef_2n2;
uniform vec3 u_SHcoef_2n1;
uniform vec3 u_SHcoef_2p0;
uniform vec3 u_SHcoef_2p1;
uniform vec3 u_SHcoef_2p2;

vec4 shadeSphere(vec3 dir)
{
	dir = dir.xzy * vec3(-1, -1, 1);

	const float c_SHconst_0 = 0.28209479177387814347f; // 1 / (2*sqrt(pi))
	const float c_SHconst_1 = 0.48860251190291992159f; // sqrt(3 /(4pi))
	const float c_SHconst_2 = 1.09254843059207907054f; // 1/2 * sqrt(15/pi)
	const float c_SHconst_3 = 0.31539156525252000603f; // 1/4 * sqrt(5/pi)
	const float c_SHconst_4 = 0.54627421529603953527f; // 1/4 * sqrt(15/pi)

	vec3 color = u_SHcoef_0p0 * c_SHconst_0;

	color += u_SHcoef_1n1 * c_SHconst_1 * dir.y;
	color += u_SHcoef_1p0 * c_SHconst_1 * dir.z;
	color += u_SHcoef_1p1 * c_SHconst_1 * dir.x;

	color += u_SHcoef_2n2 * c_SHconst_2 * (dir.x*dir.y);
	color += u_SHcoef_2n1 * c_SHconst_2 * (dir.y*dir.z);
	color += u_SHcoef_2p0 * c_SHconst_3 * (3f*dir.z*dir.z - 1f);
	color += u_SHcoef_2p1 * c_SHconst_2 * (dir.x*dir.z);
	color += u_SHcoef_2p2 * c_SHconst_4 * (dir.x*dir.x - dir.y*dir.y);

	//return vec4(-color.r, color.r, 0, 1);
	return vec4(color, 1);
}

/*
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
*/

void main()
{
	vec2 snorm_coord = vf_TexCoord * 2 - 1;
	vec3 dir = vec3(snorm_coord, 1);
	dir = normalize(u_Rotation * dir);
	out_Color = shadeSphere(dir);
}
