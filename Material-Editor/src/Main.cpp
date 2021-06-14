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

			vertexNodes[3].inputNames[0] = "noiseInputX";
			vertexNodes[4].inputNames[0] = "noiseInputY";
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

		//Shader generatedShader(vertexNodes, fragmentNodes);


		
		
		ShaderGraph graph(&lineShader);

		float time = 0;
		while (!window.shouldClose()) {
			window.beginLoop();
			
			graph.draw();
			graph.drawGenerated();

			window.endLoop();
		}
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}