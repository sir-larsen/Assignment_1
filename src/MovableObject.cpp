/**
 * @file MovableObject.cpp
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Source code for the MovableObject class.
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "MovableObject.h"

/**
 * @brief Construct a new Movable Object:: Movable Object object.
 * 
 * @param maze 			- The maze the MovableObject is to be drawn "in"
 * @param shader 		- The MovableObject's shader
 * @param renderer 		- The MovableObject's renderer
 * @param ID 			- The identifier of this current MovableObjecct
 * @param spritePaths 	- A path to the file containing the remaining sprite filepaths.
 */
MovableObject::MovableObject(Maze* maze, Shader* shader, Renderer* renderer, const int ID, const std::string spritePaths)
	:	m_Maze(maze),	
		m_Renderer(renderer),
		m_Shader(shader),
		mapID(ID),
		m_spritePaths(spritePaths),
		alive(true)
{
	direction = right; newDirection = true;
	width = m_Maze->getWidth(); height = m_Maze->getHeight();
	generateMovableObject();
}

/**
 * @brief Destroy the Movable Object:: Movable Object object.
 * 
 */
MovableObject::~MovableObject()
{
	free(movableObjectVAO);
	free(movableObjectVBO);
	free(movableObjectVBLayout);
	free(movableObjectIBO);
}

/**
 * @brief Generates a new movableobject, including the creation of the objects
 * 		  vertices and indices and the OpenGL code.
 * @see generateMovableObjectVertices();
 * @see makeMovableObjectIndices();
 */
void MovableObject::generateMovableObject()
{
	generateMovableObjectVertices();
	makeMovableObjectIndices();


	movableObjectVAO = new VertexArray;
	movableObjectVAO->Bind();
	movableObjectVBO = new VertexBuffer(&movableObjectVertices[0], movableObjectVertices.size() * sizeof(glm::vec3));
	movableObjectVBO->Bind();
	movableObjectVBLayout = new VertexBufferLayout;
	movableObjectVBLayout->Push<float>(3);
	movableObjectVBLayout->Push<float>(3);
	movableObjectVAO->AddBuffer(*movableObjectVBO, *movableObjectVBLayout);

	movableObjectIBO = new IndexBuffer(&movableObjectIndices[0], movableObjectIndices.size());

	m_Shader->createShaderProgram();

	movableObjectAnimator = new Animator(m_spritePaths);
	m_Shader->setUniform1i("u_Texture", mapID);
}

/**
 * @brief Draws the MovableObject
 * 
 */
void MovableObject::draw()
{
	m_Shader->camera(width, height);
	m_Renderer->Draw(movableObjectVAO, movableObjectIBO, m_Shader);
}

/**
 * @brief Moves (translates) the MovableObject. 
 * 
 * @param speed - The speed of the translation
 * @param dt 	- Delta time (time elapsed since the last iteration)
 */
void MovableObject::move(float speed, float dt)
{
	movableObjectAnimator->animate((int)direction, dt, mapID, newDirection);
	m_Shader->Bind();
	switch (direction)
	{
		case up:
			if (m_Maze->map2d[floor(posY)][round(posX)] != 1)
			{
				posY -= speed * dt;
				posX = round(posX); //makes the entity "snap" to the grid
			}
			break;

		case down: 
			if (m_Maze->map2d[ceil(posY)][round(posX)] != 1)
			{
				posY += speed * dt;
				posX = round(posX); //makes the entity "snap" to the grid
			}
			break;
		case left:
			if (m_Maze->map2d[round(posY)][floor(posX)] != 1)
			{
				posX -= speed * dt;
				posY = round(posY); //makes the entity "snap" to the grid
			}
			if (floor(posX) == -1) posX = 27; //makes entity reappear on opposite side
			break;
		case right:
			if (m_Maze->map2d[round(posY)][ceil(posX)] != 1)
			{
				posX += speed * dt;
				posY = round(posY); //makes the entity "snap" to the grid
			}
			if (ceil(posX) == 28) posX = 0; //makes entity reappear on opposite side
			break;
		default: break;
	}
		m_Shader->setUniformMat4("u_TransformationMat", glm::translate(glm::mat4(1.f), glm::vec3(posX-startX, posY-startY, 0.f)));
}


/**
 * @brief Generates the MovableObject's positions and texture coordinates.
 * 
 */
void MovableObject::generateMovableObjectVertices()
{
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			if (m_Maze->map2d[y][x] == mapID)
			{
				posY = startY = y; posX = startX = x;
				movableObjectVertices.push_back(glm::vec3(x, y, 0.f));     //position
				movableObjectVertices.push_back(glm::vec3(0.f, 1.f, 0.f)); //texture

				movableObjectVertices.push_back(glm::vec3(x + 1, y, 0.f)); //position
				movableObjectVertices.push_back(glm::vec3(1.f, 1.f, 0.f)); //texture

				movableObjectVertices.push_back(glm::vec3(x, y + 1, 0.f)); //position
				movableObjectVertices.push_back(glm::vec3(0.f, 0.f, 0.f)); //texture

				movableObjectVertices.push_back(glm::vec3(x + 1, y + 1, 0.f));	//position
				movableObjectVertices.push_back(glm::vec3(1.f, 0.f, 0.f));		//texture
				m_Maze->map2d[y][x] = 0; //marks the location of the entity for pellet rendering
			}
}

/**
 * @brief Selects the MovableObject's indices, as there is only 6 required for
 * 		  each object this can be manually selected.
 */
void MovableObject::makeMovableObjectIndices()
{
	movableObjectIndices.push_back(0); 
	movableObjectIndices.push_back(1);
	movableObjectIndices.push_back(2);

	movableObjectIndices.push_back(1);
	movableObjectIndices.push_back(2);
	movableObjectIndices.push_back(3);
}
