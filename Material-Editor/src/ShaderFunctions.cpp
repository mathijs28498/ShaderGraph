#include "headers/ShaderFunctions.h"

#include <unordered_set>

ShaderFunction vec2{
	"vec2",
	{},
	"",
	"",
	{ {"float", None, "0"}, {"float", None, "0"} },
	{"vec2", None }
};
ShaderFunction vec3{
	"vec3",
	{},
	"",
	"",
	{ {"float", None, "0"}, {"float", None, "0"}, {"float", None, "0"} },
	{"vec3", None }
};
ShaderFunction vec3x{
	"vec3x",
	{},
	"float vec3x(vec3 vec) {return vec3.x}\n",
	"",
	{ {"vec3", None, "vec3(0)"} },
	{"float", None }
};
ShaderFunction vec3y{
	"vec3y",
	{},
	"float vec3x(vec3 vec) {return vec3.y}\n",
	"",
	{ {"vec3", None, "vec3(0)"} },
	{"float", None }
};
ShaderFunction vec3z{
	"vec3z",
	{},
	"float vec3z(vec3 vec) {return vec3.z}\n",
	"",
	{ {"vec3", None, "vec3(0)"} },
	{"float", None }
};

ShaderFunction vec4{
	"vec4",
	{},
	"",
	"",
	{{"float", None, "0"}, {"float", None, "0"}, {"float", None, "0"}, {"float", None, "0"}},
	{"vec4", None}
};
ShaderFunction texture{
	"texture",
	{},
	"",
	"",
	{{"sampler2D", None, ""}, {"vec2", None, ""}},
	{"vec3", None }
};

ShaderFunction multFloat{
	"multFloat",
	{},
	"float multFloat(float a, float b);\n",
"\
float multFloat(float a, float b) {\n\
	return a * b;\n\
}\n",
	{{"float", None, "0"}, {"float", None, "0"}},
	{"float", None}
};
ShaderFunction multVec2Float{
	"multVec2Float",
	{},
	"vec2 multVec2Float(vec2 a, float b);\n",
"\
vec2 multVec2Float(vec2 a, float b) {\n\
	return a * b;\n\
}\n",
	{{"vec2", None, "vec2(0, 0)"}, {"float", None, "0"}},
	{"vec2", None}
};
ShaderFunction divVec2Float{
	"divVec2Float",
	{},
	"vec2 divVec2Float(vec2 a, float b);\n",
"\
vec2 divVec2Float(vec2 a, float b) {\n\
	return a / b;\n\
}\n",
	{{"vec2", None, "vec2(0, 0)"}, {"float", None, "1"}},
	{"vec2", None}
};
ShaderFunction addVec2{
	"addVec2",
	{},
	"vec2 addVec2(vec2 a, vec2 b);\n",
"\
vec2 addVec2(vec2 a, vec2 b) {\n\
	return a + b;\n\
}\n",
	{{"vec2", None, "vec2(0)"}, {"vec2", None, "vec2(0)"}},
	{"vec2", None}
};

ShaderFunction addVec3{
	"addVec3",
	{},
	"vec3 addVec3(vec3 a, vec3 b);\n",
"\
vec3 addVec3(vec3 a, vec3 b) {\n\
	return a + b;\n\
}\n",
	{{"vec3", None, "vec3(0)"}, {"vec3", None, "vec3(0)"}},
	{"vec3", None}
};

ShaderFunction sinShad{
	"sin",
	{},
	"",
	"",
	{ {"float", None, "0"} },
	{"float", None }
};
ShaderFunction cosShad{
	"cos",
	{},
	"",
	"",
	{ {"float", None, "0"} },
	{"float", None }
};

ShaderFunction time{
	"time",
	{"uniform float time;"},
	"",
	"",
	{},
	{"float", Undefined}
};
ShaderFunction texture0{
	"texture0",
	{"uniform sampler2D texture0;"},
	"",
	"",
	{},
	{"sampler2D", Undefined}
};

ShaderFunction noise{
	"noise",

	{},
"\
vec2 fade(vec2 t);\n\
vec4 permute(vec4 x);\n\
float noise(vec2 P);\n",
"\
vec2 fade(vec2 t) {return t*t*t*(t*(t*6.0-15.0)+10.0);}\n\
vec4 permute(vec4 x) { return mod(((x * 34.0) + 1.0) * x, 289.0); }\n\
float noise(vec2 P) {\n\
	vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);\n\
	vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);\n\
	Pi = mod(Pi, 289.0); // To avoid truncation effects in permutation\n\
	vec4 ix = Pi.xzxz;\n\
	vec4 iy = Pi.yyww;\n\
	vec4 fx = Pf.xzxz;\n\
	vec4 fy = Pf.yyww;\n\
	vec4 i = permute(permute(ix) + iy);\n\
	vec4 gx = 2.0 * fract(i * 0.0243902439) - 1.0; // 1/41 = 0.024...\n\
	vec4 gy = abs(gx) - 0.5;\n\
	vec4 tx = floor(gx + 0.5);\n\
	gx = gx - tx;\n\
	vec2 g00 = vec2(gx.x,gy.x);\n\
	vec2 g10 = vec2(gx.y,gy.y);\n\
	vec2 g01 = vec2(gx.z,gy.z);\n\
	vec2 g11 = vec2(gx.w,gy.w);\n\
	vec4 norm = 1.79284291400159 - 0.85373472095314 * vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11));\n\
	g00 *= norm.x;\n\
	g01 *= norm.y;\n\
	g10 *= norm.z;\n\
	g11 *= norm.w;\n\
	float n00 = dot(g00, vec2(fx.x, fy.x));\n\
	float n10 = dot(g10, vec2(fx.y, fy.y));\n\
	float n01 = dot(g01, vec2(fx.z, fy.z));\n\
	float n11 = dot(g11, vec2(fx.w, fy.w));\n\
	vec2 fade_xy = fade(Pf.xy);\n\
	vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);\n\
	float n_xy = mix(n_x.x, n_x.y, fade_xy.y);\n\
	return 2.3 * n_xy;\n\
}\n",

// TODO: Maybe remove most brackets
	{ { "vec2", Input , "TexCoord"} },
	{"float", None},
};

map<string, ShaderFunction*> shaderFunctions{
	{texture.name, &texture},
	{texture0.name, &texture0},
	{time.name, &time},
	{noise.name, &noise},
	{divVec2Float.name, &divVec2Float},
	{multVec2Float.name, &multVec2Float},
	{addVec2.name, &addVec2},
	{addVec3.name, &addVec3},
	{multFloat.name, &multFloat},
	{sinShad.name, &sinShad},
	{cosShad.name, &cosShad},
	{vec4.name, &vec4},
	{vec3.name, &vec3},
	{vec3x.name, &vec3x},
	{vec3y.name, &vec3y},
	{vec3z.name, &vec3z},
	{vec2.name, &vec2},
};


unordered_set<string> includedFunctions;
unordered_set<string> includedUniforms;

// TODO: Extend to Geometry/Tesselation shader
string getPrefix(PrefixType prefixType, GLuint shaderType) {
	if (prefixType == Input) {
		switch (shaderType) {
		case GL_VERTEX_SHADER:
			return "v";
		case GL_FRAGMENT_SHADER:
			return "f";
		}
	} else if (prefixType == Output) {
		switch (shaderType) {
		case GL_VERTEX_SHADER:
			return "f";
		}
	}
	return "";
};

string getParam(string* name, FuncParameter* param, GLuint shaderType) {
	return name->size() > 0 ? *name : getPrefix(param->type.prefix, shaderType) + param->value;
}

GraphNode::GraphNode(string functionName, string outputName, PrefixType outputPrefix) : func(shaderFunctions[functionName]) {
	inputNames.resize(func->defaultInputParams.size());
	this->outputName = outputName;
	this->outputPrefix = outputPrefix;
}

void GraphNode::appendNode(string* startOut, string* declarationsOut, string* mainOut, string* implementationsOut, GLuint shaderType) {
	string includedFunctionsStr = getPrefix(Input, shaderType) + func->name;
	if (!includedFunctions.count(includedFunctionsStr)) {
		includedFunctions.insert(includedFunctionsStr);
		for (string uniform : func->uniforms) {
			string includedUniform = getPrefix(Input, shaderType) + uniform;
			if (!includedUniforms.count(includedUniform)) {
				includedUniforms.insert(includedUniform);
				*startOut += uniform + '\n';
			}
		}
		*declarationsOut += func->declaration;
		*implementationsOut += func->implementation;
	}

	if (func->outputType.prefix != Undefined) {
		*mainOut += func->outputType.name + " " + getPrefix(outputPrefix, shaderType) + outputName + " = " + func->name + "(";
		for (size_t i = 0; i < inputNames.size() - 1; i++) {
			*mainOut += getParam(&inputNames[i], &func->defaultInputParams[i], shaderType) + ", ";
		}
		*mainOut += getParam(&inputNames.back(), &func->defaultInputParams.back(), shaderType) + ");\n";
	}
}

