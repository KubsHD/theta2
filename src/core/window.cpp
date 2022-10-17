#include "window.h"
#include <lib/Metal.hpp>

void Window::setDefault()
{
	quit = false;
	x = 0;
	y = 0;
	w = 640;
	h = 480;
}

void Window::create()
{
	// Create Window and get its Surface
	pWindow = SDL_CreateWindow("Ozon", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		w, h, SDL_WINDOW_SHOWN);

	if (pWindow == NULL)
	{
		std::cout << "pWindow is NULL!" << std::endl;
	}
	pSurface = SDL_GetWindowSurface(pWindow);
}

void Window::close()
{
	SDL_FreeSurface(pSurface);
	pSurface = NULL;
	SDL_DestroyWindow(pWindow);
	pWindow = NULL;
	SDL_Quit();
}


