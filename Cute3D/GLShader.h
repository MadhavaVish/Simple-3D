#pragma once

#include <QOpenGLExtraFunctions>

#include <string>

#include <glm/glm.hpp>

using std::string;

class GLShader : protected QOpenGLExtraFunctions
{
public:
	GLShader(const string vertexPath, const string fragmentPath);

	GLuint ID();

	//activate the shader
	void use();

	//utility functions
	void setBool(const string &name, bool value);
	
	void setInt(const std::string &name, int value);
	
	void setFloat(const std::string &name, float value);
	
	void setVec2(const std::string &name, const glm::vec2 &value);
	void setVec2(const std::string &name, float x, float y);
	
	void setVec3(const std::string &name, const glm::vec3 &value);
	void setVec3(const std::string &name, float x, float y, float z);
	
	void setVec4(const std::string &name, const glm::vec4 &value);
	void setVec4(const std::string &name, float x, float y, float z, float w);
	
	void setMat2(const std::string &name, const glm::mat2 &mat);
	
	void setMat3(const std::string &name, const glm::mat3 &mat);
	
	void setMat4(const std::string &name, const glm::mat4 &mat);

private:
	unsigned int vertex, fragment;
	GLuint id;
};

