#include "context.h"
#include "render.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

int main()
{
	context game;
	game.init(640, 480, "valhalla");
	
	// triangle, CCW
	vertex triverts[] =
	{
		{{    1, -1, 0, 1}, {0.0,0.0,1.0,0.0}, {1.0,0.0,0.0,1.0}, {0.f, 0.f}},
		{{  .1f, -1, 0, 1}, {0.0,0.0,1.0,0.0}, {0.0,1.0,0.0,1.0}, {1.f, 0.f}},
		{{ 0.5f,  1, 0, 1}, {0.0,0.0,1.0,0.0}, {0.0,0.0,1.0,1.0}, {0.5f, 1.f}}
		
		
	};

	unsigned int triindices[] = { 0,1,2 };

	geometry triangle = makeGeometry(triverts, 3, triindices, 3);

	const char * basicVert =
		"#version 430\n"
		"layout (location = 0) in vec4 position;\n"
		"layout (location = 1) in vec4 normal;\n"
		"layout (location = 2) in vec4 color;\n"
		"layout (location = 3) in vec2 uv;\n"
		"out vec4 vColor;\n"
		"layout (location = 0) uniform mat4 proj;\n"
		"layout (location = 1) uniform mat4 view;\n"
		"layout (location = 2) uniform mat4 model;\n"
		"out vec2 vUV;\n"
		"out vec3 vNormal;\n"
		"void main() { gl_Position = proj * view * model * position; vColor = color; vUV = uv; vNormal = normalize(model * normal).xyz; }";

	const char * basicFrag =
		"#version 430\n"
		"layout (location = 3) uniform sampler2D albedo;\n"
		"layout (location = 4) uniform vec3 lightDir;\n"
		"in vec2 vUV;\n "
		"in vec3 vNormal;\n "
		"in vec4 vColor;\n"
		"out vec4 vertColor;\n"
		"void main() {float d = max(0,dot(vNormal, -lightDir)); vec4 diffuse = d * vec4(1,1,1,1); vec4 base = texture(albedo,vUV); vertColor = diffuse * base;}";

	shader basicshader = makerShader(basicVert, basicFrag);

	glm::mat4 triModel = glm::identity<glm::mat4>();

	glm::mat4 camProj = glm::perspective(glm::radians(45.f), 640.f / 480.f, 0.1f, 100.0f);
	glm::mat4 camView = glm::lookAt(glm::vec3(0, 0, -3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	texture triTex = loadTexture("Res/toucan.png");

	light sun;
	sun.direction = glm::vec4{ -1,0,0,1 };

	while (!game.shouldClose()) 
	{
		game.tick();
		game.clear();

		triModel = glm::rotate(triModel, glm::radians(1.f), glm::vec3(0, 1, 0));
		

		
		setUniform(basicshader, 0, camProj);
		setUniform(basicshader, 1, camView);
		setUniform(basicshader, 2, triModel);
		setUniform(basicshader, 3, triTex, 0);
		setUniform(basicshader, 4, sun.direction);
		


		draw(basicshader, triangle);
	}


	game.term();

	return 0;
}