#pragma once
#include <string>
#include <GL/glew.h>

namespace GameEngine
{
	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

		void linkShaders();

		void addAttribute(const std::string& attributeName);

		GLint getUniformLocation(const std::string uniformName);

		void use();

		void unuse();

	private:

		int _numAttributes;

		void compileShader(const std::string filePath, GLuint shaderID);
		//id that refers to entire program
		GLuint _programID;

		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;


	};
}

