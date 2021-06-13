#pragma once

#include "ShaderFunctions.h"
#include "Shader.h"
#include "headers/Globals.h"

#include <vector>

#include "vendor/imgui/imgui_impl_opengl3.h"
#include "vendor/imgui/imgui_impl_glfw.h"

class ShaderGraph;
class ShaderConnection;

class ImSpecialNode {
public:
	ImSpecialNode(std::string name, ImVec2 startPos, ShaderGraph* graph) : name(name), graph(graph) {
		ImGui::Begin(name.c_str());
		ImGui::SetWindowPos(startPos);
		ImGui::End();
	}

	void draw();

	ShaderGraph* graph;
	std::string name;
	std::vector<FuncParameter> inputParams;
	std::vector<FuncParameter> outputParams;
};

class ImBeginNode : public ImSpecialNode {
public:
	ImBeginNode(ShaderGraph* graph) : ImSpecialNode("Begin", { 0, HEIGHT / 2 }, graph) {
		outputParams = {
			{ "vec3", None, "vPosition" },
			{ "vec2", None, "vTexCoord" },
			{ "vec3", None, "vNormal" },
			{ "mat4", None, "mvp" } 
		};
	}
};

class ImTransitionNode : public ImSpecialNode {
public:
	ImTransitionNode(ShaderGraph* graph) : ImSpecialNode("Vert to frag", { WIDTH / 2 - 210 / 2, HEIGHT / 2 }, graph) {
		inputParams = {
		{"vec4", None, "gl_Position"},
		{"vec2", None, "fTexCoord"},
		{"vec3", None, "fNormal"}
		};
		outputParams = {
			{"vec2", None, "fTexCoord"},
			{"vec3", None, "fNormal"}
		};
	}
};

class ImEndNode : public ImSpecialNode {
public:
	ImEndNode(ShaderGraph* graph) : ImSpecialNode("End", { WIDTH - 210, HEIGHT / 2 }, graph) {
		inputParams = {
			{"vec3", None, "FragColor"}
		};
	}
};

class ImGraphNode {
public:
	ImGraphNode(ShaderGraph* graph, GraphNode node);
	void draw();

	bool activeNode = true;
	ShaderGraph* graph;
	std::string name;
	GraphNode node;
	std::vector<std::vector<float>> inputValues;

private:
	void onInputButton(FuncParameter param, int i);
};

class ShaderConnection {
public:
	ShaderConnection(Shader* shader, std::vector<float> positions, ImGraphNode* beginNode) :
		shader(shader), positions(positions), endNode(nullptr) {
		setBeginNode(beginNode);
		createVAO();
	};

	void updateCursorPosition();
	void updateFirstCoord(glm::vec2 delta);
	void updateSecondCoord(glm::vec2 delta);
	void draw();

	ImGraphNode* getBeginNode() {
		return beginNode;
	}
	ImGraphNode* getEndNode() {
		return endNode;
	}

	void setBeginNode(ImGraphNode* beginNode) {
		this->beginNode = beginNode;

		ImGui::Begin(beginNode->name.c_str());
		oldWindowPosBegin = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
		ImGui::End();
	}

	void setEndNode(ImGraphNode* endNode) {
		this->endNode = endNode;

		ImGui::Begin(endNode->name.c_str());
		oldWindowPosEnd = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
		ImGui::End();
	}

private:
	Shader* shader;
	uint32_t VAO, VBO;
	std::vector<float> positions;
	glm::vec2 oldWindowPosBegin;
	glm::vec2 oldWindowPosEnd;
	ImGraphNode* beginNode;
	ImGraphNode* endNode;

	void createVAO();
	void updateVAO();
};

class ShaderGraph {
public:
	ShaderConnection* selectedConnection;
	bool selectedConnectionPress = false;

	ShaderGraph(Shader* lineShader) : lineShader(lineShader) {};
	void removeNode(ImGraphNode* node);
	void removeConnectionWithBeginNode(ImGraphNode* node);
	void removeConnectionWithEndNode(ImGraphNode* node, int i = -1);
	void addNode(GraphNode node);
	void addConnection(ImVec2 startPos, ImGraphNode* beginNode);
	void draw();

private:
	Shader* lineShader;
	bool firstDraw = true;

	std::vector<ImGraphNode*> nodes;
	std::vector<ShaderConnection*> connections;
	std::vector<ImSpecialNode*> specialNodes;
};

