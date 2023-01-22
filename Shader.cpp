#include "Shader.h"

Shader::Shader(const std::string& vert, const std::string& frag) {
	vertex = LoadShader(vert);
	fragment = LoadShader(frag);

	CreateShader(vertex, GL_VERTEX_SHADER, vertShader);
	CreateShader(fragment, GL_FRAGMENT_SHADER, fragShader);

	void LinkShader();
}

std::string Shader::LoadShader(const std::string& filepath) {

	std::ifstream stream(filepath);
	std::stringstream ss;
	ss << stream.rdbuf();
	return ss.str();

}

void Shader::CreateShader(const std::string& source, GLenum type, GLuint& shader) {

	shader = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

}

void Shader::LinkShader() {

	// Create the shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

}

void Shader::UseShader() {

	// Use the shader program
	glUseProgram(shaderProgram);

}

void Shader::DeleteShader() {

	// Use the shader program
	glDeleteProgram(shaderProgram);

}