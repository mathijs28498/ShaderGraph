#include "headers/Shader.h"
#include "headers/ShaderFunctions.h"

#include <map>

std::string generateVertexCode(std::vector<GraphNode> nodes) {
	std::string start =
		"#version 330 core\n"
		"layout(location = 0) in vec3 vPosition;\n"
		"layout(location = 1) in vec2 vTexCoord;\n"
		"layout(location = 2) in vec3 vNormal;\n"
		"\n"
		"out vec2 fTexCoord;\n"
		"out vec3 fNormal;\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 proj;\n"
		"\n";

	std::string declarations = "\n";

	std::string main = 
		"void main() {\n"
		"mat4 mvp = proj * view * model;\n";
		

	std::string implementations =
		"\n";

	for (GraphNode node : nodes) {
		node.appendNode(&start, &declarations, &main, &implementations, GL_VERTEX_SHADER);
	}
	main +=
		"	gl_Position = proj * view * model * vec4(vPosition.x + noiseX, vPosition.y + noiseY, vPosition.z, 1.0);\n"
		"	fTexCoord = vTexCoord;\n"
		"	fNormal = vNormal;\n"
		"}\n";

	return start + declarations + main + implementations;
}

std::string generateFragmentCode(std::vector<GraphNode> nodes) {
	std::string start =
		"#version 330 core\n"
		"in vec2 fTexCoord;\n"
		"in vec3 fNormal;\n"
		"\n"
		"out vec4 FragColor;\n"
		"\n"
		"uniform sampler2D texture0;\n"
		"\n";

	std::string declarations =
		"\n";

	std::string main = "void main() {\n";

	std::string implementations =
		"\n";

	for (GraphNode node : nodes) {
		node.appendNode(&start, &declarations, &main, &implementations, GL_FRAGMENT_SHADER);
	}

	main += 
		"FragColor = texture(texture0, fTexCoord) * vec4(noiseHeight, 0, 1 - noiseHeight, 1);\n"
		"}\n";
	

	return start + declarations + main + implementations;
}

Shader::Shader(std::vector<GraphNode> vertexNodes, std::vector<GraphNode> fragmentNodes) {
	std::string vertexCode = generateVertexCode(vertexNodes);
	std::string fragmentCode = generateFragmentCode(fragmentNodes);

	//std::cout << vertexCode << '\n';
	//std::cout << fragmentCode << '\n';

	uint32_t vertex = createShader(vertexCode.c_str(), GL_VERTEX_SHADER);
	uint32_t fragment = createShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	int success;
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		throw std::runtime_error("failed to link shaders!");
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	uint32_t vertex = createShader(getShaderCode(vertexPath).c_str(), GL_VERTEX_SHADER);
	uint32_t fragment = createShader(getShaderCode(fragmentPath).c_str(), GL_FRAGMENT_SHADER);

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	int success;
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		throw std::runtime_error("failed to link shaders!");
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::Shader(const char* vertexPath, const char* geometryPath, const char* fragmentPath) {
	uint32_t vertex = createShader(getShaderCode(vertexPath).c_str(), GL_VERTEX_SHADER);
	uint32_t geometry = createShader(getShaderCode(geometryPath).c_str(), GL_GEOMETRY_SHADER);
	uint32_t fragment = createShader(getShaderCode(fragmentPath).c_str(), GL_FRAGMENT_SHADER);

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, geometry);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	int success;
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		throw std::runtime_error("failed to link shaders!");
	}

	glDeleteShader(vertex);
	glDeleteShader(geometry);
	glDeleteShader(fragment);
}

uint32_t Shader::createShader(const char* shaderCode, GLuint type) {
	uint32_t shaderID;
	int success;
	char infoLog[512];

	shaderID = glCreateShader(type);
	glShaderSource(shaderID, 1, &shaderCode, NULL);
	glCompileShader(shaderID);

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cout << infoLog << '\n';
		std::cout << shaderCode << '\n';
		throw std::runtime_error("failed to compile shader!");
	}
	return shaderID;
}

std::string Shader::getShaderCode(const char* shaderPath) {
	std::ifstream shaderFile;

	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();

		return shaderStream.str().c_str();
	} catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ " << shaderPath << '\n';
		std::cout << e.what() << '\n';
	}
}

void Shader::use() {
	glUseProgram(ID);
}


