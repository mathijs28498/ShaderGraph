#version 330 core
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexCoord;
layout(location = 2) in vec3 vNormal;

out vec2 fTexCoord;
out vec3 fNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

vec4 multMat4Vec3(mat4 mat, vec3 vec) {return mat * vec4(vec.xyz, 1);};
void main() {
mat4 mvp = proj * view * model;
vec4 A = multMat4Vec3(mvp, vPosition);
        gl_Position = A;
        fTexCoord = vTexCoord;
        fNormal = vNormal;
}