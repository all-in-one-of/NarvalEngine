#pragma once
#include <glad/glad.h>
#include "Texture2D.h"
#include <glm/glm.hpp>

class FBO
{
public:
	GLuint id;

	void bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, id);
	}

	void unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);   
	}

	void clear(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void clear(glm::vec4 color) {
		clear(color.x, color.y, color.z, color.a);
	}

	void clear() {
		clear(0, 0, 0, 0);
	}

	void attachTexture(int id) {
		bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);
		unbind();
	}

	void attachDepthAndStencil(int id) {
		bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, id, 0);
		unbind();
	}

	void attachTexture(Texture2D texture) {
		attachTexture(texture.id);
	}

	FBO();
	~FBO();
};

