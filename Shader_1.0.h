#ifndef SHADER_H
#define SHADER_H

#include <glad\glad.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

class Shader
{
public:
	// program ID
	unsigned int program;

	// constructor
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
	{
		std::string vertexCode, fragmentCode;
		std::ifstream vShaderfile, fShaderfile;
		vShaderfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			vShaderfile.open(vertexPath);
			fShaderfile.open(fragmentPath);
			std::stringstream vShaderstream, fShaderstream;
			vShaderstream << vShaderfile.rdbuf();
			fShaderstream << fShaderfile.rdbuf();

			vShaderfile.close();
			fShaderfile.close();

			vertexCode = vShaderstream.str();
			fragmentCode = fShaderstream.str();

		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		std::cout << vertexCode << "\n" << fragmentCode << std::endl;

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		// compile shader
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		fragment = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	// use function
	void use()
	{
		glUseProgram(program);
	}

	// uniform function
	void setBool(const std::string &name, bool value)const
	{
		glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
	}

	void setInt(const std::string &name, int value)const
	{
		glUniform1i(glGetUniformLocation(program, name.c_str()), value);
	}

	void setFloat(const std::string &name, float value)const
	{
		glUniform1f(glGetUniformLocation(program, name.c_str()), value);
	}
};

#endif