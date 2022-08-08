/**
 * @file Shader.cpp
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief The source file for the Shader class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "Shader.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
/**
 * @brief Enum for determining shader type.
 * 
 */
enum class ShaderType {
	NONE = -1, VERTEX = 0, FRAGMENT = 1
};

/**
 * @brief Construct a new Shader:: Shader object.
 * 
 * @param filepath - Filepath to the file containing the shaders
 */
Shader::Shader(const std::string& filepath)
{
	std::ifstream shaderFile(filepath);

	std::string line;
	std::stringstream sstream[2];
	ShaderType type = ShaderType::NONE;
	while (getline(shaderFile, line))
	{
		if (line.find("#shader") != std::string::npos) //if the line is a shader declaration
		{
			if (line.find("vertex") != std::string::npos) //if it is a vertex shader
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos) //if it is a fragment shader
				type = ShaderType::FRAGMENT;
		}
		else //the line is actual shader code
		{
			sstream[(int)type] << line << '\n'; //read the shader code
		}
	}
	vertexShaderSource = sstream[0].str();
	fragmentShaderSource = sstream[1].str();
}

/**
 * @brief Destroy the Shader:: Shader object.
 * 
 */
Shader::~Shader()
{
	glDeleteProgram(shaderProgram);
}

/**
 * @brief Creates a Vertex Shader.
 * 
 */
void Shader::createVertexShader()
{
	const char* src = vertexShaderSource.c_str();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &src, NULL);
	glCompileShader(vertexShader);


	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION::FAILED\n" << infoLog << std::endl;
	}
}

/**
 * @brief Creates a fragment shader.
 * 
 */
void Shader::createFragmentShader()
{

	const char* src = fragmentShaderSource.c_str();
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &src, NULL);
	glCompileShader(fragmentShader);

	int success;
	char infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION::FAILED\n" << infoLog << std::endl;
	}
}

/**
 * @brief Creates a Shader program.
 * 
 * @return shaderProgram - The newly created shaderprogram's ID.
 */
unsigned int Shader::createShaderProgram()
{
	createVertexShader();		//creates the vertex shader
	createFragmentShader();		//creates the fragment shader

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	//checking if the shaderprogram linked successfully
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING::FAILED\n" << infoLog << std::endl;
	}
	else
		glUseProgram(shaderProgram);

	//shaders have been linked into a program, and can therefore be deleted
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}


/**
 * @brief Sets the camera for the shader.
 * 
 * @param horizontalSize - The horizontal size (width) of the screen
 * @param verticalSize 	 - The vertical size (height) of the screen
 */
void Shader::camera(int horizontalSize, int verticalSize)
{
	glm::mat4 projection = glm::ortho(0.f, (float)horizontalSize, (float)verticalSize, 0.f);

	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	GLuint projmat = glGetUniformLocation(shaderProgram, "u_ProjectionMat");
	GLuint viewmat = glGetUniformLocation(shaderProgram, "u_ViewMat");

	glUniformMatrix4fv(projmat, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewmat, 1, GL_FALSE, glm::value_ptr(view));

}

/**
 * @brief Binds (selects) the current shaderProgram.
 * 
 */
void Shader::Bind() const
{
	glUseProgram(shaderProgram);
}

/**
 * @brief Unbinds the current shaderProgram.
 * 
 */
void Shader::Unbind() const
{
	glUseProgram(0);
}

/**
 * @brief Sets a uniform containing 4 floats.
 * 
 * @param name 	- Name of the uniform
 * @param v0 	- float 1
 * @param v1 	- float 2
 * @param v2 	- float 3
 * @param v3 	- float 4
 */
void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	int location = getUniformLocation(name);
	glUniform4f(location, v0, v1, v2, v3);
}

/**
 * @brief Sets a uniform for a mat4 (matrix 4x4).
 * 
 * @param name The name of the mat4
 * @param mat  The actual mat4 that is to be sent to the uniform (shader)
 */
void Shader::setUniformMat4(const std::string& name, glm::highp_mat4 mat)
{
	int location = getUniformLocation(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

/**
 * @brief Sets a uniform containing one int.
 * 
 * @param name 	- the name of the uniform
 * @param value - the one int that is to be sent with
 */
void Shader::setUniform1i(const std::string& name, int value)
{
	glUniform1i(getUniformLocation(name), value);
}

/**
 * @brief Finds the location of a uniform.
 * 
 * @param name - The name of the uniform that is to be found.
 * @return location - The actual location of the uniform if it has been found, else -1
 */
int Shader::getUniformLocation(const std::string& name)
{
	int location = glGetUniformLocation(shaderProgram, name.c_str());
	if (location == -1)
		std::cout << "Warning: Uniform " << name << " does not exist!\n";
	return location;
}

