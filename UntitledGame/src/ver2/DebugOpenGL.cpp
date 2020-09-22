#include "DebugOpenGL.h"

void debugFramebuffer(GLenum target)
{
	GLenum result = glCheckFramebufferStatus(target);

	switch (result)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		std::cout << "OK_GL: The specified framebuffer is complete." << std::endl;
		break;
	case GL_FRAMEBUFFER_UNDEFINED:
		std::cout << "ERROR_GL: Any of the framebuffer attachment points are framebuffer incomplete." << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		std::cout << "ERROR_GL: The framebuffer does not have at least one image attached to it." << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		std::cout << "ERROR_GL: The value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi." << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		std::cout << "ERROR_GL: GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER." << std::endl;
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		std::cout << "ERROR_GL: The combination of internal formats of the attached images violates an implementation-dependent set of restrictions." << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		std::cout << "ERROR_GL: The value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers." << std::endl;
		std::cout << "ERROR_GL: The value of GL_TEXTURE_SAMPLES is the not same for all attached textures." << std::endl;
		std::cout << "ERROR_GL: The attached images are a mix of renderbuffers and textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES." << std::endl;
		std::cout << "ERROR_GL: The value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures." << std::endl;
		std::cout << "ERROR_GL: The attached images are a mix of renderbuffers and textures, the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures." << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		std::cout << "ERROR_GL: any framebuffer attachment is layered, and any populated attachment is not layered, or if all populated color attachments are not from textures of the same target." << std::endl;
		break;
	case 0:
		std::cout << "ERROR_GL: An error occured." << std::endl;
		break;
	}
}

