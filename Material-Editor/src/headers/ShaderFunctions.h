#pragma once

#include <string>
#include <vector>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;


enum PrefixType {
	None,
	Input,
	Output,
	Undefined
};

struct VarType {
	string name;
	PrefixType prefix;
};

struct FuncParameter {
	VarType type;
	string value;
};

// TODO: Make sure that start doesn't have overlapping elements
struct ShaderFunction {
	string name;
	vector<string> uniforms;
	string declaration;
	string implementation;
	vector<FuncParameter> defaultInputParams;
	VarType outputType;
};

class GraphNode {
public:
	vector<string> inputNames;
	ShaderFunction* func;
	string outputName;
	PrefixType outputPrefix;

	GraphNode(string functionName, string outputName, PrefixType outputPrefix);
	void appendNode(string* startOut, string* declarationsOut, string* mainOut, string* implementationsOut, GLuint shaderType);
};

extern map<string, ShaderFunction*> shaderFunctions;