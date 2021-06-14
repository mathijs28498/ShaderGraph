#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

constexpr uint32_t WIDTH = 1500, HEIGHT = 900;

uint32_t createTestVAO(); 
uint32_t loadTexture(const char* imagePath, GLenum format, bool shouldFlip);