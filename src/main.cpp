#include <iostream>
#include <SDL.h>

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#include <lib/Metal.hpp>

//#include <core/network.h>
#include "core/log.h"
#include "core/window.h"

#include "platform/macos/metal_helper.h"

bool bRunning = true;

SDL_Event evt;
SDL_Window* win;

Window window;

CA::MetalLayer* swapchain;
MTL::Device* device;
MTL::CommandQueue* queue;

SDL_MetalView view;

void init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) > 0)
		log_error("Error: %s", SDL_GetError());

	
}

void render()
{
	MTL::ClearColor color(0.2, 0.3, 0.4, 1);
	
	auto surface = next_drawable(SDL_Metal_GetLayer(view));
	auto pass_descriptor = MTL::RenderPassDescriptor::alloc()->init();
	auto attachment = pass_descriptor->colorAttachments()->object(0);
	attachment->setClearColor(color);
	attachment->setLoadAction(MTL::LoadActionClear);
	attachment->setTexture(surface->texture());

	auto buffer = queue->commandBuffer();
	auto encoder = buffer->renderCommandEncoder(pass_descriptor);
	encoder->endEncoding();
	buffer->presentDrawable(surface);
	buffer->commit();
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	init();

	// Keeping game running at 60 fps
	const float MS = 16.6666666666667f;
	double last = SDL_GetTicks64();
	double lag = 0.0;
	double current = 0;

	window.setDefault();
	window.create();

	view = SDL_Metal_CreateView(window.pWindow);
	device = MTL::CreateSystemDefaultDevice();
	queue = device->newCommandQueue();

	assign_device(SDL_Metal_GetLayer(view), device);

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

		//input_update();
		//audio_update();
		render();
	}

	return 0;
}
