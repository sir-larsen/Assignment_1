/**
 * @file Ghost.cpp
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Source code for the Ghost class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "Ghost.h"
#include "MovableObject.h"
#include "GLFW/glfw3.h"

#include <iostream>

/**
 * @brief Construct a new Ghost:: Ghost object
 * 
 * @param maze 			- The maze the Ghost is "in"
 * @param shader 		- The Ghost's shader
 * @param renderer 		- The Ghost's renderer
 * @param ID 			- The Ghost's identifier in the level
 * @param spritePaths 	- The path to the file containing the paths to the rest of the sprites.
 */
Ghost::Ghost(Maze* maze, Shader* shader, Renderer* renderer, const int ID, const std::string spritePaths)
	: MovableObject(maze,shader,renderer,ID,spritePaths)
{
	elapsedTime = 0;
	dir = 3;
}

/**
 * @brief Generates a new direction based on the current direction
 * @see	Ghost::randomize();
 */
void Ghost::getInput()
{
	if (elapsedTime > 0.3f)
	{
		elapsedTime = 0;
		newDirection = true;
		switch (dir)
		{
		case 0: direction = up;			randomize(vertical);	 break;
		case 1: direction = down;		randomize(vertical);	 break;
		case 2: direction = right;		randomize(horizontal);	 break;
		case 3: direction = left;		randomize(horizontal);	 break;
		default:												 break;
		}
		prevPosX = posX; prevPosY = posY;
	}
}

/**
 * @brief Moves the Ghost
 * @see getInput()
 * @param speed - The speed of the movement
 * @param dt 	- Delta time
 */
void Ghost::move(float speed, float dt)
{
	elapsedTime += dt;
	getInput();
	MovableObject::move(speed, dt);
}

/**
 * @brief Generates a "random" new direction
 * 
 * @param axis - The axis the ghost is currently traversing 
 */
void Ghost::randomize(Axis axis)
{
	double prevPos = 0, Pos = 0;
	switch (axis)
	{
	case vertical:
		prevPos = prevPosY; Pos = posY; break;
	case horizontal:
		prevPos = prevPosX; Pos = posX; break;
	}
	if (prevPos == Pos) {
		dir = rand() % 4;
	}
}

