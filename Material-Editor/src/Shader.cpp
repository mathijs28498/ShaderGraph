#include "headers/Shader.h"
#include "headers/ShaderFunctions.h"

#include <map>

std::string generateVertexCode(std::vector<GraphNode> nodes, SpecialShaderNode* specialNode) {
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
	for (size_t i = 0; i < specialNode->inputParams.size(); i++) {
		if (specialNode->inputNames[i] != "") {
			main += specialNode->inputParams[i].value + " = " + specialNode->inputNames[i] + ";\n";
		}
	}
	main += "}\n";

	return start + declarations + main + implementations;
}

std::string generateFragmentCode(std::vector<GraphNode> nodes, SpecialShaderNode* specialNode) {
	std::string start =
		"#version 330 core\n"
		"in vec2 fTexCoord;\n"
		"in vec3 fNormal;\n"
		"\n"
		"out vec4 FragColor;\n";

	std::string declarations =
		"\n";

	std::string main = "void main() {\n";

	std::string implementations =
		"\n";

	for (GraphNode node : nodes) {
		node.appendNode(&start, &declarations, &main, &implementations, GL_FRAGMENT_SHADER);
	}

	for (size_t i = 0; i < specialNode->inputParams.size(); i++) {
		if (specialNode->inputNames[i] != "") {
			main += specialNode->inputParams[i].value + " = " + specialNode->inputNames[i] + ";\n";
		}
	}
	main += "}\n";
	

	return start + declarations + main + implementations;
}

Shader::Shader(std::vector<GraphNode> vertexNodes, std::vector<GraphNode> fragmentNodes, std::vector<SpecialShaderNode> specialNodes) {
	resetShaderFunctionSets();
	std::string vertexCode = generateVertexCode(vertexNodes, &specialNodes[0]);
	std::string fragmentCode = generateFragmentCode(fragmentNodes, &specialNodes[1]);

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


