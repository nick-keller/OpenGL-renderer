#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Scene.h"
#include "Shader.h"
#include "Engine.h"

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
	SDL_GL_SetSwapInterval(0);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	SDL_DisplayMode desktop;
	SDL_GetDesktopDisplayMode(0, &desktop);

	window = SDL_CreateWindow("Test SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, desktop.w /1.5, desktop.h /1.5, SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);

	glContext = SDL_GL_CreateContext(window);
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(0, 0, 0, 1);

	Engine engine = Engine(desktop.w /1.5, desktop.h /1.5);
	engine.init();

	do
	{
		time = SDL_GetTicks();
		delta = time - prevTime;
		prevTime = time;

		while (SDL_PollEvent(&events)) {
			engine.update(0, events);
		}
		engine.update(delta, events);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		engine.render();

		SDL_GL_SwapWindow(window);

	} while (events.window.event != SDL_WINDOWEVENT_CLOSE);

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}