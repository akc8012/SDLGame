#include "../Header/ShaderProgram.h"
#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

ShaderProgram::ShaderProgram()
{
	shaderProgramId = glCreateProgram();

	uint vertexShaderId = createVertexShader();
	glAttachShader(shaderProgramId, vertexShaderId);

	uint fragmentShaderId = createFragmentShader();
	glAttachShader(shaderProgramId, fragmentShaderId);

	int success;
	glLinkProgram(shaderProgramId);
	glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(shaderProgramId, 512, NULL, infoLog);
		throw (string)"Error linking program: " + infoLog;
	}

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
}

uint ShaderProgram::createVertexShader()
{
	const char* vertexShaderSource = {
		"#version 330 core\n"
		"layout (location = 0) in vec3 vertexPos;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(vertexPos.x, vertexPos.y, vertexPos.z, 1.0);\n"
		"}\n"
	};

	return createShader(GL_VERTEX_SHADER, readShaderSourceFromFile("Resource/Shader/VertexShader.shader").c_str());
}

uint ShaderProgram::createFragmentShader()
{
	const char* fragmentShaderSource = {
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(0.0f, 1.0f, 0.3f, 1.0f);\n"
		"}\n"
	};

	return createShader(GL_FRAGMENT_SHADER, readShaderSourceFromFile("Resource/Shader/FragmentShader.shader").c_str());
}

uint ShaderProgram::createShader(uint type, const char* source)
{
	uint shaderId = glCreateShader(type);
	glShaderSource(shaderId, 1, &source, NULL);

	int success;
	glCompileShader(shaderId);
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		throw (string)"Unable to compile shader: " + infoLog;
	}

	return shaderId;
}

string ShaderProgram::readShaderSourceFromFile(const char* filePath)
{
	ifstream file;
	file.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		file.open(filePath);
	}
	catch (ifstream::failure e)
	{
		throw("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
	}

	stringstream stream;
	stream << file.rdbuf();
	
	file.close();
	return stream.str();
}

uint ShaderProgram::getShaderProgramId()
{
	return shaderProgramId;
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(shaderProgramId);
}
