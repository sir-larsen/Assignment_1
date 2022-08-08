#include "src/ScenarioLoader.h"
#include "src/Shader.h"

#include "src/Maze.h"
#include "src/MovableObject.h"
#include "src/Pacman.h"
#include "src/Ghost.h"
#include "src/Pellets.h"
#include "src/stb_image.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include <iomanip>


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 1024, "Assigment One - Pacman", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);


	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	GLFWimage images[2];
	images[0].pixels = stbi_load("res/ghost/ghost_right0.png", &images[0].width, &images[0].height, 0, 4);
	images[1].pixels = stbi_load("res/ghost/ghost_right0.png", &images[1].width, &images[1].height, 0, 4);
	glfwSetWindowIcon(window, 1, images);

	stbi_image_free(images[0].pixels);
	stbi_image_free(images[1].pixels);

	Shader mazeShader("shaders/level01.glsl"); //Parses shaders from file
	Shader pacmanShader("shaders/pacman.glsl");
	Shader pelletsShader("shaders/pellets.glsl");

	ScenarioLoader mazeScenario("levels/level0"); //loads the level from file

	Renderer renderer;


	Maze maze(&mazeScenario, &mazeShader, &renderer); //generates the maze

	std::vector <Shader*> ghostShaders;
	std::vector <MovableObject*> ghosts;
	for (int i = 0; i < 3; i++)
	{
		ghostShaders.push_back(new Shader("shaders/ghost.glsl"));
		ghosts.push_back(new Ghost(&maze, ghostShaders[i], &renderer, 3 + i, "res/ghost/ghost"));
	}
	
	MovableObject* pacman = new Pacman(&maze, &pacmanShader, &renderer,2,window,"res/pacman/pacman",ghosts);
	
	Pellets pellets(&maze, &pelletsShader, &renderer,pacman);
	
	int maxPellets = pellets.getScore(); bool havePrintedFinalScore = false;

	srand(time(NULL)); //sets a random seed for future rand() calls.
	double dt = 0;
	glfwSetTime(0);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		if (pacman->alive && !pellets.allPelletsGone())
		{
			dt = glfwGetTime();
			glfwSetTime(0); //sets time to 0 to check elapsed time until next glfwGetTime()
		}
		else
		{
			if (!havePrintedFinalScore)
			{
				havePrintedFinalScore = true;
				if (pellets.getScore() == 0)
					std::cout << "Congratulations! You ate all pellets!\n";
				else
					std::cout << "Game over! You ate " << maxPellets - pellets.getScore() << '/' << maxPellets << " pellets.\n";
				std::cout << "Press ESC to exit the application...\n";
			}
			dt = 0;
		}
		/* Render here */
		renderer.Clear(.1f, .1f, .1f, 1.f);
		
		maze.draw();
		pellets.draw();

		pacman->move(4.f, dt);
		pacman->draw();


		for (int i = 0; i < ghosts.size(); i++)
		{
			ghosts[i]->move(4.f, dt);
			ghosts[i]->draw();
		}

		glfwSwapBuffers(window);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			break;
		}

		/* Poll for and process events */
		glfwPollEvents();


	}

	glfwTerminate();
	return 0;
}