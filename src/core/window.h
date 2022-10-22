#pragma once
#include <SDL.h>
#include <iostream>
#include <lib/stb_image.h>


class Window {
public:
	int x, y, w, h;
	bool fullscreen;
	bool quit;
	SDL_Window* pWindow;

	SDL_Surface* pSurface;

	Window();
	void create();
	void close();
	void update();
	bool loadTextureIMG();
};