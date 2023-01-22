#pragma once
#include <glad/glad.h>
#include <fstream>
#include <sstream>

class Shader
{
	private:
		std::string vertex;
		std::string fragment;
		GLuint vertShader;
		GLuint fragShader;
		GLuint shaderProgram;
	public:
		Shader(const std::string& vert, const std::string& frag);
		~Shader(){}
		void UseShader();
		void DeleteShader();
	private:
		std::string LoadShader(const std::string& filepath);
		void CreateShader(const std::string& source, GLenum type, GLuint& shader);
		void LinkShader();

};

