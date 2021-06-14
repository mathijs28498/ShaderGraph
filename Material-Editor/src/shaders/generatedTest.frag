#version 330 core
in vec2 fTexCoord;
in vec3 fNormal;

out vec4 FragColor;
uniform sampler2D texture0;

sampler2D texture0Func() {return texture0;};
void main() {
	sampler2D test = texture0Func();
	sampler2D B = texture0Func();
	vec3 C = texture(B, fTexCoord);
	FragColor = C;
}