/**
 * @file Pacman.cpp
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief The source code for the Pacman class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "Pacman.h"
#include "MovableObject.h"
#include <iostream>

/**
 * @brief Construct a new Pacman:: Pacman object
 * 
 * @param maze 			- The maze Pacman is to be drawn "in"
 * @param shader 		- Pacman's shader
 * @param renderer 		- Pacman's renderer
 * @param ID 			- Pacman's identifier
 * @param window 		- The window that is going to be used for input detection later.
 * @param spritePaths 	- The path to the file containing the filepaths of the rest of the sprites.
 * @param ghosts		- vector containing all ghosts, used for collision detection
 */
Pacman::Pacman(Maze* maze, Shader* shader, Renderer* renderer, const int ID, GLFWwindow* window, const std::string spritePaths, std::vector <MovableObject*> ghosts)
	: MovableObject(maze, shader, renderer, ID, spritePaths),
	  m_Ghosts(ghosts)
{
	m_Window = window;
}

/**
 * @brief Destroy the Pacman:: Pacman object
 * 
 */
Pacman::~Pacman()
{
	free(m_Window);
}

/**
 * @brief Fetches input from the player
 * 
 */
void Pacman::getInput()
{
	if (glfwGetKey(m_Window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		direction = up;
		newDirection = true;
	}
	if (glfwGetKey(m_Window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		direction = down;
		newDirection = true;
	}
	if (glfwGetKey(m_Window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		direction = right;
		newDirection = true;
	}
	if (glfwGetKey(m_Window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		direction = left;
		newDirection = true;
	}
}

/**
 * @brief Moves pacman
 * 
 * @param speed - The speed of movement
 * @param dt 	- Delta time
 */
void Pacman::move(float speed, float dt)
{
	this->getInput();
	isAlive();
	if (alive)
		MovableObject::move(speed, dt);
}

/**
 * @brief checks wheter or not pacman has been caught by the ghosts.
*/
void Pacman::isAlive()
{
	for (int i = 0; i < m_Ghosts.size(); i++)
		if (round(posX) == round(m_Ghosts[i]->posX) && round(posY) == round(m_Ghosts[i]->posY))
			alive = false;
}
