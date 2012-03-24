#version 330 core

out vec2 vf_TexCoord;

void main() {
	vf_TexCoord = vec2((gl_VertexID & 1) << 1, 1 - (gl_VertexID & 2));
	gl_Position = vec4((vf_TexCoord * 2 - 1) * vec2(1, 800f/600f), 0, 1);
}
