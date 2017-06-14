#ifndef Engine_ShaderProgram_hpp
#define Engine_ShaderProgram_hpp

#include <glew.h>
#include <gl/GL.h>

#include <Core/ResourceManager.h>

#include <lib/Helper.hpp>

namespace Engine
{
	class ShaderProgram
	{
	public:
		ShaderProgram() { _programID = glCreateProgram(); };
		~ShaderProgram() {};

		void CompileShader(const char* source, GLenum shaderType);

		GLuint GetProgramID() { return _programID; }
	private:
		GLuint _programID;
		std::vector<std::string> sources;
	};

	inline void ShaderProgram::CompileShader(const char* source, GLenum shaderType) {
		sources.push_back(source);

		ResourceManager* RM = ResourceManager::GetInstance();
		Resource* asd = RM->LoadResource(source);

		std::string temp = asd->getTextData();
		const char* fuck = temp.c_str();

		GLuint shaderID = glCreateShader(shaderType);
		glShaderSource(shaderID, 1, &fuck, 0);

		glCompileShader(shaderID);

		glAttachShader(_programID, shaderID);

		glLinkProgram(_programID);

		GLAssert();
	}
}

#endif