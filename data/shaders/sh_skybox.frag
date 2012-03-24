#version 330 core

in vec2 vf_TexCoord;

layout(location = 0) out vec4 out_Color;

vec4 shadeSphere(vec3 dir)
{
	return vec4(dir, 1);
}

void main()
{
	out_Color = vec4(vf_TexCoord.st, 0, 1);
}
