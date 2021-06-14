#version 330 core
in vec2 fTexCoord;
in vec3 fNormal;

out vec4 FragColor;

float vec3x(vec3 vec) {return vec.x;};
void main() {
float AAAA = vec3x(vec3(1.000000,0.000000,0.000000));
vec4 AAAB = vec4(AAAA, 0.000000, 0.000000, 1.000000);
FragColor = AAAB;
}