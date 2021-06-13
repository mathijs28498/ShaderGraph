#include "headers/ShaderGraph.h"
#include "headers/ShaderFunctions.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include <iostream>

constexpr float FLOAT_INPUT_WIDTH = 50;
constexpr float START_WINDOW_WIDTH = 210;

std::string getUniqueName() {
	static uint16_t i = 0;
	char name[]{
		((i & 0xf000) >> 12) + 65,
		((i & 0x0f00) >> 8) + 65,
		((i & 0x00f0) >> 4) + 65,
		((i & 0x000f) >> 0) + 65,
		0x00
	};
	i++;
	std::string temp = std::string(name);
	return name;
}

void ImSpecialNode::draw() {
	ImGui::StyleColorsClassic();
	ImGui::Begin(name.c_str(), (bool*) 0, ImGuiWindowFlags_NoCollapse);
	ImGui::SetWindowSize({ START_WINDOW_WIDTH, 0 }); 
	for (FuncParameter output : outputParams) {
		int letterAmount = output.value.size() + output.type.name.size() + 1;
		ImGui::SameLine(START_WINDOW_WIDTH - letterAmount * 7 - 35); ImGui::Text(output.value.c_str());
		ImGui::SameLine(); ImGui::Text(output.type.name.c_str());
		ImGui::SameLine(); 
		if (ImGui::ArrowButton(output.value.c_str(), ImGuiDir_Right)) {
			//graph->addConnection(ImGui::GetMousePos(), output);
		}
		ImGui::NewLine();
	}
	for (FuncParameter input : inputParams) {
		if (ImGui::ArrowButton(input.value.c_str(), ImGuiDir_Left)) {

		}
		ImGui::SameLine(); ImGui::Text(input.type.name.c_str());
		ImGui::SameLine(); ImGui::Text(input.value.c_str());
	}
	ImGui::End();
	ImGui::StyleColorsDark();
}

ImGraphNode::ImGraphNode(ShaderGraph* graph, GraphNode node) : graph(graph), node(node) {
	name = node.func->name + " (" + node.outputName + ')';

	for (FuncParameter param : node.func->defaultInputParams) {
		inputValues.push_back({ 0, 0, 0, 0 });
	}

	ImGui::Begin(name.c_str(), &activeNode);
	ImGui::SetWindowPos({ WIDTH / 2, HEIGHT / 2 });
	ImGui::End();
}

void ImGraphNode::onInputButton(FuncParameter param, int i) {
	ShaderConnection* con = graph->selectedConnection;
	if (con != nullptr && con->getBeginNode()->node.func->outputType.name == param.type.name) {
		if (node.inputNames[i] != "")
			graph->removeConnectionWithEndNode(this, i);

		con->setEndNode(this);
		node.inputNames[i] = con->getBeginNode()->name;
		graph->selectedConnection = nullptr;
		graph->selectedConnectionPress = false;
	} else if (con == nullptr) {
		graph->removeConnectionWithEndNode(this, i);
		node.inputNames[i] = "";
	}
}

void ImGraphNode::draw() {
	ImGui::SetNextWindowBgAlpha(0.7f);
	ImGui::Begin(name.c_str(), &activeNode, ImGuiWindowFlags_NoCollapse);
	if (!activeNode)
		graph->removeNode(this);

	float windowWidth = START_WINDOW_WIDTH;
	ImGui::Text(node.func->name.c_str());
	
	int nextItemWidth = node.func->outputType.name == "sampler2D" ? 100 : 65;
	ImGui::SameLine(START_WINDOW_WIDTH - nextItemWidth); ImGui::Text((node.func->outputType.name).c_str());
	ImGui::SameLine();
	if (ImGui::ArrowButton("output", ImGuiDir_Right)) {
		// TODO: Change this
		graph->addConnection(ImGui::GetMousePos(), this);
	}

	for (int i = 0; i < node.func->defaultInputParams.size(); i++) {
		FuncParameter param = node.func->defaultInputParams[i];
		if (ImGui::ArrowButton("input " + i, ImGuiDir_Left)) {
			onInputButton(param, i);
		}
		// ERROR: The inputfloat sometimes becomes inactive causing the inputs for floats to not be changable. 
		// This seems to be a bug in the ImGui framework! https://github.com/ocornut/imgui/issues/840
		ImGui::SameLine();
		if (node.inputNames[i] == "") {
			ImGui::PushItemWidth(FLOAT_INPUT_WIDTH * 2);
			const char* inputName = (param.type.name + ' ' + to_string(i)).c_str();
			if (param.type.name == "float") {
				ImGui::InputFloat(inputName, inputValues[i].data(), 0.1f, 1, "%.2f");
			} else if (param.type.name == "vec2") {
				ImGui::InputFloat2(inputName, inputValues[i].data(), "%.2f");
			} else if (param.type.name == "vec3") {
				ImGui::InputFloat3(inputName, inputValues[i].data(), "%.2f");
			} else if (param.type.name == "vec4") {
				ImGui::InputFloat4(inputName, inputValues[i].data(), "%.2f");
			} else {
				ImGui::SameLine(FLOAT_INPUT_WIDTH * 2 + 39);
				ImGui::Text(inputName);
			}
		} else {
			ImGui::Text(node.inputNames[i].substr(0, 14).c_str());
			ImGui::SameLine(FLOAT_INPUT_WIDTH * 2 + 39);
			ImGui::Text(param.type.name.c_str());
		}
	}
	ImGui::SetWindowSize({ windowWidth, 0 });

	ImGui::End();
}


void ShaderConnection::createVAO() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}


void ShaderConnection::updateVAO() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ShaderConnection::draw() {
	shader->use();
	shader->setVec2("screenSize", { WIDTH, HEIGHT });
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
	glBindVertexArray(0);
	if (beginNode != nullptr) {
		ImGui::Begin(beginNode->name.c_str());

		glm::vec2 newWindowPos(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
		glm::vec2 deltaWindowPos = newWindowPos - oldWindowPosBegin;
		if (glm::length2(deltaWindowPos) > 0.01f) {
			updateFirstCoord(deltaWindowPos);
		}
		oldWindowPosBegin = newWindowPos;

		ImGui::End();
	}
	if (endNode != nullptr) {
		ImGui::Begin(endNode->name.c_str());

		glm::vec2 newWindowPos(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
		glm::vec2 deltaWindowPos = newWindowPos - oldWindowPosEnd;
		if (glm::length2(deltaWindowPos) > 0.01f) {
			updateSecondCoord(deltaWindowPos);
		}
		oldWindowPosEnd = newWindowPos;

		ImGui::End();
	}
}

void ShaderConnection::updateCursorPosition() {
	positions[2] = ImGui::GetMousePos().x;
	positions[3] = ImGui::GetMousePos().y;
	updateVAO();
}

void ShaderConnection::updateFirstCoord(glm::vec2 delta) {
	positions[0] = positions[0] + delta.x;
	positions[1] = positions[1] + delta.y;
	updateVAO();
}

void ShaderConnection::updateSecondCoord(glm::vec2 delta) {
	positions[2] = positions[2] + delta.x;
	positions[3] = positions[3] + delta.y;
	updateVAO();
}

void ShaderGraph::addNode(GraphNode node) {
	nodes.push_back(new ImGraphNode{ this, node });
}

void ShaderGraph::removeNode(ImGraphNode* node) {
	removeConnectionWithBeginNode(node);
	removeConnectionWithEndNode(node);
	nodes.erase(std::remove(nodes.begin(), nodes.end(), node), nodes.end());
}

void ShaderGraph::removeConnectionWithBeginNode(ImGraphNode* node) {
	for (ShaderConnection* con : connections) {
		if (con->getBeginNode() == node) {
			ImGraphNode* endNode = con->getEndNode();
			for (size_t i = 0; i < endNode->node.inputNames.size(); i++) {
				if (endNode->node.inputNames[i] == node->name) {
					endNode->node.inputNames[i] = "";
				}
			}
		}
	}

	connections.erase(remove_if(begin(connections), end(connections), [node](ShaderConnection* u) {
		return u->getBeginNode() == node;
		}), end(connections));
}

void ShaderGraph::removeConnectionWithEndNode(ImGraphNode* node, int i) {
	connections.erase(remove_if(begin(connections), end(connections), [node, i](ShaderConnection* u) {
		return u->getEndNode() == node && (i < 0 || u->getBeginNode()->name == node->node.inputNames[i]);
		}), end(connections));
}



void ShaderGraph::draw() {
	if (firstDraw) {
		specialNodes = {
			new ImBeginNode(this),
			new ImTransitionNode(this),
			new ImEndNode(this)
		};
		firstDraw = false;
	}

	ImGui::Begin("ShaderGraph", (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowSize({ 0, HEIGHT / 2 - 50 });
	ImGui::SetWindowPos({ 0, 0 });
	if (ImGui::Button("GENERATE")) {
		//TODO: Make logic to generate code
	}
	ImGui::NewLine();
	for (auto funcPair : shaderFunctions) {
		ShaderFunction* func = funcPair.second;
		if (ImGui::Button(func->name.c_str())) {
			addNode({ func->name, getUniqueName(), None });
		}
	}
	ImGui::End();
	for (ImSpecialNode* node : specialNodes) {
		node->draw();
	}
	for (ImGraphNode* node : nodes) {
		node->draw();
	}
	for (ShaderConnection* con : connections) {
		con->draw();
	}
	if (selectedConnection != nullptr) {
		selectedConnection->updateCursorPosition();
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			selectedConnectionPress = true;
		}
		if (selectedConnection != nullptr && selectedConnectionPress && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
			connections.pop_back();
			//delete selectedConnection;
			selectedConnection = nullptr;
			selectedConnectionPress = false;
			// TODO: Remove the selected connection
		}
	}
}

void ShaderGraph::addConnection(ImVec2 startPos, ImGraphNode* begin) {
	connections.push_back(new ShaderConnection{ lineShader, { startPos.x, startPos.y, startPos.x, startPos.y }, begin });
	selectedConnection = connections.back();
}