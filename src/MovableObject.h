/**
 * @file MovableObject.h
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Header file for the MovableObject class 
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once
#include "Maze.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Animator.h"

#include <iostream>

/**
 * @brief The direction the MovableObject is currently moving.
 * 
 */
enum Direction
{
	up = 0,
	down = 1,
	right = 2,
	left = 3
};

/**
 * @class MovableObject
 * @brief The class that is responsible for everything related to MovableObjects, such as pacman, ghosts, etc. 
 */
class MovableObject
{
private:

	int height,
		width,
		mapID;

	double	startX,
			startY;

	std::vector <unsigned int>	movableObjectIndices;
	std::vector <glm::vec3>		movableObjectVertices;

	Maze* m_Maze;
	Renderer* m_Renderer;
	Shader* m_Shader;

	VertexArray*		movableObjectVAO;
	VertexBuffer*		movableObjectVBO;
	VertexBufferLayout* movableObjectVBLayout;
	IndexBuffer*		movableObjectIBO;

	Animator*			movableObjectAnimator;
	std::string			m_spritePaths;
public:
	MovableObject(Maze* maze, Shader* shader, Renderer* renderer,const int ID, const std::string spritePaths);
	~MovableObject();
	
	void generateMovableObject();
	void draw();
	virtual void move(float speed, float dt);
	virtual void getInput() = 0;

private:
	void generateMovableObjectVertices();
	void makeMovableObjectIndices();

public:
	Direction	direction;
	bool		newDirection, alive;
	double		posX,
				posY;
};
