#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <stdexcept>

class Window {
public:
	GLFWwindow* window;

	Window(int width, int height, const char* title, glm::vec4 backgroundColor);
	~Window();
	bool shouldClose();

	void beginLoop();
	void endLoop();

private:
	glm::vec4 backgroundColor;
	void initImGui();
};