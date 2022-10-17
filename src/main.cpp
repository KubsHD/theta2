#include <iostream>
#include <SDL.h>
#include <lib/glad/glad.h>

//#include <core/network.h>
#include "core/log.h"
#include "core/window.h"

#include <d3d11.h>       // D3D interface
#include <dxgi.h>        // DirectX driver interface
#include <d3dcompiler.h> // shader compiler

#pragma comment( lib, "user32" )          // link against the win32 library
#pragma comment( lib, "d3d11.lib" )       // direct3D library
#pragma comment( lib, "dxgi.lib" )        // directx graphics interface
#pragma comment( lib, "d3dcompiler.lib" ) // shader compiler

#include <core/network.h>

bool bRunning = true;
static SDL_GLContext maincontext;

SDL_Event evt;
SDL_Window* win;

Window window;

void init()
{
	// Initializing SDL2
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		log_error("Error: %s", SDL_GetError());
	}

	// Request an OpenGL 4.5 context (should be core)
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG );
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Initializing window
	window.create();

	//SDL_GL_MakeCurrent(window.pWindow, maincontext);

	maincontext = SDL_GL_CreateContext(window.pWindow);
	if (maincontext == NULL)
		log_error("Failed to create OpenGL context");

	// Initializing OpenGL
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		std::cout << "Failed to initialize GLAD. " << std::endl;
	}
}

void render()
{

}

/// Makes game run in 60 fps, calls update functions on different scenes
int main(int argc, char* argv[])
{
	srand(time(NULL));

	init();

	// Keeping game running at 60 fps
	const float MS = 16.6666666666667f;
	double last = SDL_GetTicks64();
	double lag = 0.0;
	double current = 0;

	

	
	while (bRunning)
	{
		while (SDL_PollEvent(&evt) != 0)
		{
			switch (evt.type)
			{
			case SDL_KEYDOWN:
				if (evt.key.keysym.sym == SDLK_ESCAPE)
				{
					bRunning = false;
					window.close();
				}
				break;
			case SDL_QUIT:
				bRunning = false;
				window.close();
				break;
			case SDL_CONTROLLERDEVICEADDED:
				//input_notify_pad_added();
				break;
			case SDL_CONTROLLERDEVICEREMOVED:
				//input_notify_pad_removed();
				break;
			}
		}

		//ImGui_ImplSDL2_ProcessEvent(&event);

		current = SDL_GetTicks64();
		double dt = current - last;
		last = current;
		lag += dt;


		while (lag < MS)
		{
			int milliseconds = (int)(MS - lag);
			SDL_Delay(milliseconds);

			current = SDL_GetTicks64();
			dt = current - last;
			last = current;
			lag += dt;
		}

		while (lag >= MS)
		{
			//update(dt);
			lag -= MS;
		}

		glClearColor(.2f, .5f, .0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);



		SDL_GL_SwapWindow(window.pWindow);
		//input_update();
		//audio_update();
		render();
#if !OFFLINE
		net_update();
#endif
	}

	return 0;
}
