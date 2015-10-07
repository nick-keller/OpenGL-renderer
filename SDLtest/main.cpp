#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Scene.h"
#include "Shader.h"

int WinMain()
{
	SDL_Window* window(0);
	SDL_GLContext glContext(0);
	SDL_Event events;
	Uint32 time(0), prevTime(0), delta(0);

	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	window = SDL_CreateWindow("Test SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	glContext = SDL_GL_CreateContext(window);
	glewInit();

	glEnable(GL_DEPTH_TEST);

	Camera camera = Camera(glm::vec3(10, 10, 10), glm::vec3(-1, -1, -1));
	Scene scene = Scene(800.0/600.0);

	do
	{
		SDL_PollEvent(&events);
		time = SDL_GetTicks();
		delta = time - prevTime;
		prevTime = time;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.rotate(.01, glm::vec3(0, 0, 1));
		scene.render(camera);

		SDL_GL_SwapWindow(window);

	} while (events.window.event != SDL_WINDOWEVENT_CLOSE);

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}