#include "Texture.h"
#include <lib\Helper.hpp>

namespace Engine {

	Texture::Texture(const char* source)
	{
		tex = ResourceManager::GetInstance()->LoadResource(source);

		// Create one OpenGL texture
		glGenTextures(1, &texid);

		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, texid);

		// Give the image to OpenGL
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->iWidth, tex->iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &tex->getImageData()[0]);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	void Texture::Init()
	{

	}

	void Texture::Cleanup()
	{

	}

	void Texture::Update(DeltaTime deltaTime)
	{

	}

	void Texture::BindTexture(GLint id)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texid);
	}

	void Texture::RenderReflection()
	{
		//glViewport(0, 0, texSize, texSize);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glLoadIdentity();
		//gluLookAt(......)

		//	glPushMatrix();
		//glTranslatef(0.0f, 0.0f, 0.0f);
		//glScalef(1.0, -1.0, 1.0);
		//double plane[4] = { 0.0, 1.0, 0.0, 0.0 }; //water at y=0
		//glEnable(GL_CLIP_PLANE0);
		//glClipPlane(GL_CLIP_PLANE0, plane);
		//RenderScene();
		//glDisable(GL_CLIP_PLANE0);
		//glPopMatrix();

		////render reflection to texture
		//glBindTexture(GL_TEXTURE_2D, reflection);
		////glCopyTexSubImage2D copies the frame buffer
		////to the bound texture
		//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, texSize, texSize);
	}
}