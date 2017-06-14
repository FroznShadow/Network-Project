#include <Core\Systems\Buffer.hpp>
#include <glm\glm.hpp>
#include <lib\Helper.hpp>

namespace Engine {
	void Buffer::CreateBuffer(GLenum bufferType) {
		_bufferType = bufferType;

		switch (_bufferType)
		{
		case GL_ARRAY_BUFFER: {
			glGenBuffers(1, &_bufferID);
			glBindBuffer(_bufferType, _bufferID);
			glBufferData(_bufferType, _bufferSize, NULL, GL_DYNAMIC_DRAW);
			GLAssert();
			break;
		}
		case GL_ELEMENT_ARRAY_BUFFER: {
			glGenBuffers(1, &_bufferID);
			glBindBuffer(_bufferType, _bufferID);
			glBufferData(_bufferType, _bufferSize, NULL, GL_DYNAMIC_DRAW);
			GLAssert();
			break;
		}
		default: {
			std::cout << "Unrecognized buffer type!" << std::endl;
			break;
		}
		}
	}




	void Buffer::BindBufferData(GLuint size, void* data) {
		switch (_bufferType)
		{
		case GL_ARRAY_BUFFER: {
			if (size * sizeof(glm::vec3) > _bufferSize) { Resize(size); }
			glBufferSubData(_bufferType, 0, sizeof(glm::vec3) * size, data);
			GLAssert();
			break;
		}
		case GL_ELEMENT_ARRAY_BUFFER: {
			if (size * sizeof(glm::uvec3) > _bufferSize) { Resize(size); }
			glBufferSubData(_bufferType, 0, sizeof(glm::uvec3) * size, data);
			GLAssert();
			break;
		}
		default: {
			printf_s("Unrecognized buffer type!\n");
			break;
		}
		}
	}

	void Buffer::Resize(GLuint size) {

		switch (_bufferType)
		{
		case GL_ARRAY_BUFFER: {
			while (_bufferSize < size * sizeof(glm::vec3)) { _bufferSize *= 2; }
			while (_bufferSize / 2 > size * sizeof(glm::vec3)) { _bufferSize /= 2; }
			GLAssert();
			glBufferData(_bufferType, _bufferSize, 0, GL_DYNAMIC_DRAW);
			GLAssert();
			break;
		}
		case GL_ELEMENT_ARRAY_BUFFER: {
			while (_bufferSize < size * sizeof(glm::uvec3)) { _bufferSize *= 2; }
			while (_bufferSize / 2 > size * sizeof(glm::vec3)) { _bufferSize /= 2; }
			GLAssert();
			glBufferData(_bufferType, _bufferSize, 0, GL_DYNAMIC_DRAW);
			GLAssert();
			break;
		}
		default: {
			printf_s("Undefined buffer type!\n");
			break;
		}
		}
	}
}