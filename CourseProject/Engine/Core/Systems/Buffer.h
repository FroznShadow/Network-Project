#ifndef Engine_Buffer_hpp
#define Engine_Buffer_hpp

#include <iostream>
#include <glew.h>

namespace Engine
{
	class Buffer
	{
	public:
		Buffer() : _bufferID(0), _bufferSize(1), _bufferType(GL_NONE) {};
		~Buffer() {};

		void CreateBuffer(GLenum bufferType);
		void BindBufferData(GLuint size, void* data);
		void Resize(GLuint size);

	private:
		//void Resize(GLuint size);

		GLuint _bufferID;
		GLuint _bufferSize;
		GLenum _bufferType;
	};
}

#endif