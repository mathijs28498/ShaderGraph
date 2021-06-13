#version 330 core
layout (location = 0) in vec2 position;

uniform vec2 screenSize;

void main() {
    vec2 newPos = vec2(position.x / (screenSize.x / 2) - 1, -(position.y / (screenSize.y / 2) - 1));
    gl_Position = vec4(newPos, 0.0, 1.0);
}