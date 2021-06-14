#include "headers/Globals.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <vector>

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