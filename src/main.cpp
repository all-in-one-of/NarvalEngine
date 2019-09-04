#include "Engine3D.h"
#include "GameStateManager.h"
#include "ResourceManager.h"
#include <iostream>
#include <vector>


void generateTestModel(){
	float vertices[] = {
		-1, -1, 0,
		1, -1, 0,
		1, 1, 0,

		-1, -1, 0,
		1, 1, 0,
		-1, 1, 0
	};

	float texcoords[] = {
		0, 1,
		1, 1,
		1, 0,

		0, 1,
		1, 0,
		0, 0
	};

	std::vector<GLfloat> *values = new std::vector<GLfloat>();
	std::vector<int> *layout = new std::vector<int>();
	(*layout).push_back(3);
	(*layout).push_back(2);

	for (int i = 0; i < 6; i++) {
		(*values).push_back(vertices[(i * 3)]);
		(*values).push_back(vertices[(i * 3) + 1]);
		(*values).push_back(vertices[(i * 3) + 2]);
		(*values).push_back(texcoords[(i * 2)]);
		(*values).push_back(texcoords[(i * 2) + 1]);
	}


	Model model;
	model.loadVerAttrib(&(*values).front(), (*values).size(), &(*layout).front(), (*layout).size());
	ResourceManager::getSelf()->addModel("quadTest", model);
}

void generateCubeTestModel() {
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};

	float texcoords[] = {
		 0.0f,  0.0f,
		1.0f,  0.0f,
		1.0f,  1.0f,
		1.0f,  1.0f,
		 0.0f,  1.0f,
		 0.0f,  0.0f,

		 0.0f,  0.0f,
		1.0f,  0.0f,
		1.0f,  1.0f,
		1.0f,  1.0f,
		 0.0f,  1.0f,
		 0.0f,  0.0f,

		 1.0f,  0.0f,
		 1.0f,  1.0f,
		 0.0f,  1.0f,
		 0.0f,  1.0f,
		 0.0f,  0.0f,
		 1.0f,  0.0f,

		1.0f,  0.0f,
		1.0f,  1.0f,
		0.0f,  1.0f,
		0.0f,  1.0f,
		0.0f,  0.0f,
		1.0f,  0.0f,

		 0.0f,  1.0f,
		1.0f,  1.0f,
		1.0f,  0.0f,
		1.0f,  0.0f,
		 0.0f,  0.0f,
		 0.0f,  1.0f,

		 0.0f,  1.0f,
		1.0f,  1.0f,
		1.0f,  0.0f,
		1.0f,  0.0f,
		 0.0f,  0.0f,
		 0.0f,  1.0f
	};

	std::vector<GLfloat> *values = new std::vector<GLfloat>();
	std::vector<int> *layout = new std::vector<int>();
	(*layout).push_back(3);
	(*layout).push_back(2);

	for (int i = 0; i < 6 * 6; i++) {
		(*values).push_back(vertices[(i * 3)]+0.5);
		(*values).push_back(vertices[(i * 3) + 1]+0.5);
		(*values).push_back(vertices[(i * 3) + 2]+0.5);
		(*values).push_back(texcoords[(i * 2)]);
		(*values).push_back(texcoords[(i * 2) + 1]);
	}


	Model model;
	model.loadVerAttrib(&(*values).front(), (*values).size(), &(*layout).front(), (*layout).size());
	ResourceManager::getSelf()->addModel("cubeTest", model);
}

int main(){	
	Engine3D *engine = new Engine3D();
	engine->init();

	GameStateManager gsm;
	ResourceManager::getSelf()->loadShader("monocolor", "shaders/monoColor.vert", "shaders/monoColor.frag", "");
	ResourceManager::getSelf()->loadShader("cloudscape", "shaders/cloudscape.vert", "shaders/cloudscape.frag", "");
	ResourceManager::getSelf()->loadShader("volume", "shaders/volume.vert", "shaders/volume.frag", "");
	ResourceManager::getSelf()->loadTexture2D("cloudheights", "imgs/heights.png");
	ResourceManager::getSelf()->loadTexture2D("weather", "imgs/weather.png");
	ResourceManager::getSelf()->loadShader("screentex", "shaders/screenTex.vert", "shaders/screenTex.frag", "");
	ResourceManager::getSelf()->loadShader("gradientBackground", "shaders/gradientBackground.vert", "shaders/gradientBackground.frag", "");

	generateCubeTestModel();
	generateTestModel();

	engine->mainLoop();
}
