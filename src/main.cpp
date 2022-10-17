#include <iostream>
#include <SDL.h>

#include <d3d11.h>       // D3D interface
#include <dxgi.h>        // DirectX driver interface
#include <d3dcompiler.h> // shader compiler

#pragma comment( lib, "user32" )          // link against the win32 library
#pragma comment( lib, "d3d11.lib" )       // direct3D library
#pragma comment( lib, "dxgi.lib" )        // directx graphics interface
#pragma comment( lib, "d3dcompiler.lib" ) // shader compiler

#include <core/network.h>

bool bRunning = true;

SDL_Event evt;
SDL_Window* win;

ID3D11Device* device;
IDXGIDevice* dev;

void init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	win = SDL_CreateWindow("theta2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
}

void render()
{

}

/// Makes game run in 60 fps, calls update functions on different scenes
int main(int argc, char* argv[])
{
#if !OFFLINE
	net_init();
#endif

	srand(time(NULL));

	// Variables for keeping game running at 60 fps
	const float MS = 16.6666666666667f;
	double last = SDL_GetTicks64();
	double lag = 0.0;
	double current = 0;

	// enemies
	//bool test_enemy = enemy_wave_parser();

	init();

	while (bRunning)
	{

		while (SDL_PollEvent(&evt) != 0)
		{
			switch (evt.type)
			{
			case SDL_QUIT:
				bRunning = false;
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

		// spowalniamy giere jak za szybko dziala
		// tldr: ma byc 60 fps(16.6ms) i koniec kropka
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

		//input_update();
		//audio_update();
		render();
#if !OFFLINE
		net_update();
#endif
	}

	net_shutdown();
	return 0;
}
