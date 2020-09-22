#include "CreatePointShadow.h"

CreatePointShadow::CreatePointShadow()
{
	pointShadowContainer.shadowCube = createTexture();
	pointShadowContainer.fbo = createFbo();
}

GLuint CreatePointShadow::createTexture()
{
	GLuint shadowMap;

	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			POINT_SHADOW_WIDTH_, POINT_SHADOW_HEIGHT_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	return shadowMap;
}

GLuint CreatePointShadow::createFbo()
{
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, pointShadowContainer.shadowCube, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	debugFramebuffer(GL_FRAMEBUFFER);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return fbo;
}
