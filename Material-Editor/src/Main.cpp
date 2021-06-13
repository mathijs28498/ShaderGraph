#include "myExternals/Window.h"
#include "headers/Shader.h"
#include "headers/ShaderFunctions.h"
#include "headers/ShaderGraph.h"
#include "headers/Globals.h"

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

uint32_t loadTexture(const char* imagePath, GLenum format, bool shouldFlip) {
	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(shouldFlip);
	uint8_t* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		throw std::runtime_error("failed to load texture\n");
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
	return texture;
}


uint32_t createTestVAO() {
	std::vector<float> coords = {
		// Back
		1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,		0.0f, 0.0f,

		// Front
		1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,		0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,		1.0f, 0.0f,

	};

	std::vector<uint32_t> indices = {
		0, 2, 1,
		0, 3, 2,

		5, 4, 6,
		7, 6, 4,

		5, 1, 2,
		1, 5, 6,

		//1, 2, 6,
		//6, 5, 1,
		//   	  
		//   	  
		//2, 3, 6,
		//6, 3, 7,
		//   	  
		//0, 7, 3,
		//0, 4, 7,
		//   	  
		//0, 1, 5,
		//0, 5, 4,
	};

	uint32_t VAO, VBO, IBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(float), coords.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	return VAO;
}


int main() {
	try {
		//Window window(WIDTH, HEIGHT, "Material fun", { 0.45f, 0.55f, 0.60f, 1.00f });
		Window window(WIDTH, HEIGHT, "Material fun", { 0, 0, 0, 1.00f });
		Shader lineShader("src/shaders/line.vert", "src/shaders/line.frag");
		//Shader generatedShader("src/shaders/generated.vert", "src/shaders/generated.frag");

		std::vector<GraphNode> vertexNodes{
			{"vec2", "noiseInputX", None},
			{"multFloat", "vec2InputY", None},
			{"vec2", "noiseInputY", None},
			{"noise", "noiseX", None},
			{"noise", "noiseY", None},
			{"time", "time", None}
		};
		{
			vertexNodes[0].inputNames[0] = "time";

			vertexNodes[1].inputNames[0] = "time";
			vertexNodes[1].inputNames[1] = "2";

			vertexNodes[2].inputNames[0] = "vec2InputY";
			vertexNodes[3].inputNames[0] = "vec2(0,0)";
			vertexNodes[4].inputNames[0] = "vec2(0,0)";

			//vertexNodes[3].inputNames[0] = "noiseInputX";
			//vertexNodes[4].inputNames[0] = "noiseInputY";
		}

		std::vector<GraphNode> fragmentNodes{
			{"vec2", "timeV2", None},
			{"divVec2Float", "timeV2_2", None},
			{"addVec2", "newCoord", None},
			{"multVec2Float", "noiseInput", None},
			{"noise", "noiseHeight", None},
			{"time", "time", None}
		};
		{
			fragmentNodes[0].inputNames[0] = "time";

			fragmentNodes[1].inputNames[0] = "timeV2";
			fragmentNodes[1].inputNames[1] = "10";

			fragmentNodes[2].inputNames[0] = "fTexCoord";
			fragmentNodes[2].inputNames[1] = "timeV2_2";

			fragmentNodes[3].inputNames[0] = "newCoord";
			fragmentNodes[3].inputNames[1] = "10";

			fragmentNodes[4].inputNames[0] = "noiseInput";
		}

		Shader generatedShader(vertexNodes, fragmentNodes);


		uint32_t VAO = createTestVAO();

		glm::mat4 proj = glm::perspective(glm::radians(45.0f), (WIDTH + 0.0f) / HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = glm::lookAt({ 0.5, 0.5, 5 }, { 0.5, 0.5, 0.5 }, glm::vec3(0.0f, 1.0f, 0.0f));
		uint32_t texture = loadTexture("textures/brick_wall.jpg", GL_RGB, true);
		
		ShaderGraph graph(&lineShader);

		float time = 0;
		while (!window.shouldClose()) {
			window.beginLoop();
			float deltaTime = 1 / ImGui::GetIO().Framerate;
			time += deltaTime;

			// TODO: Make this into a function to use in shaderGraph
			generatedShader.use();
			generatedShader.setMat4("model", glm::mat4(1.0f));
			generatedShader.setMat4("view", view);
			generatedShader.setMat4("proj", proj);
			generatedShader.setFloat("time", time);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
			glBindVertexArray(0);
			graph.draw();

			window.endLoop();
		}
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}