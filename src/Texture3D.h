#pragma once
#include <glad/glad.h>
#include <stdio.h>

class Texture3D
{
public:
	Texture3D();
	Texture3D(int width, int height, int depth);
	~Texture3D();
	GLuint id;
	//float *data;

	void generateWithData(int width, int height, int depth, int nmrChannels, float *data) {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_3D, id);
		//this->data = data;

		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
		if (nmrChannels == 1)
			glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, width, height, depth, 0, GL_RED, GL_FLOAT, data);
		if(nmrChannels == 3)
			glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB8, width, height, depth, 0, GL_RGB, GL_FLOAT, data);
		if(nmrChannels == 4)
			glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, width, height, depth, 0, GL_RGBA, GL_FLOAT, data);

		glBindTexture(GL_TEXTURE_3D, 0);
	}

	void bind() {
		glBindTexture(GL_TEXTURE_3D, id);
	}

private:
	int width, height, depth;
};

