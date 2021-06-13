#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoord;
layout (location = 2) in vec3 vNormal;

out vec2 fTexCoord;
out vec3 fNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform float time;

void main() {
	gl_Position = proj * view * model * vec4(vPosition, 1.0);
	fTexCoord = vTexCoord;
	fNormal = vNormal;
}