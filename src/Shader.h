/**
 * @file Shader.h
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Header file for the Shader class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once

#include <string>
#include <glm/glm.hpp>

/**
 * @class Shader
 * @brief Boilerplate OpenGL code regarding the creation, and administration of shaders.  
 */
class Shader
{
private:
	std::string vertexShaderSource;
	std::string fragmentShaderSource;
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
public:
	Shader(const std::string& filepath);
	~Shader();
	void createVertexShader();
	void createFragmentShader();
	unsigned int createShaderProgram();
	void camera(int horizontalSize, int verticalSize);

	void Bind() const;
	void Unbind() const;

	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniformMat4(const std::string& name, glm::highp_mat4);
	void setUniform1i(const std::string& name, int value);
	int getUniformLocation(const std::string& name);

	unsigned int getVertexShader() { return vertexShader; }
	unsigned int getFragmentShader() { return fragmentShader; }
	unsigned int getShaderProgram() { return shaderProgram; }
	std::string* getVertexShaderSource() { return &vertexShaderSource; }
	std::string* getFragmentShaderSource() { return &fragmentShaderSource; }
};