#include <iostream>
#include <stdio.h>
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
MTL::Library* lib;
MTL::RenderPipelineState* pipelineState;
MTL::Buffer* vb;

SDL_MetalView view;

void init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) > 0)
		log_error("Error: %s", SDL_GetError());



	const char* shaderSrc = R"(
        #include <metal_stdlib>
        using namespace metal;

        float4 vertex vs_main( uint vid [[vertex_id]],
                               device const float3* positions [[buffer(0)]]
                                )
        {
            return float4(positions[vid], 1.0);
        }

        half4 fragment fs_main( )
        {
            return half4(1.0);
        }
    )";

	float vertexData[] =
    {
         -0.8f,  0.8f, 0.0f,
          0.0f, -0.8f, 0.0f,
          0.8f,  0.8f, 0.0f
	};

	vb = device->newBuffer(vertexData, sizeof(vertexData) * sizeof(float), MTL::ResourceStorageModeManaged);
	NS::Error* err;
	lib = device->newLibrary( NS::String::string(shaderSrc, NS::StringEncoding::UTF8StringEncoding), nullptr, &err);
	if ( !lib )
    {
        log_error( "%s", err->localizedDescription()->utf8String() );
        assert( false );
    }

	MTL::RenderPipelineDescriptor* pDesc = MTL::RenderPipelineDescriptor::alloc()->init();

	pDesc->setVertexFunction(lib->newFunction(NS::String::string("vs_main", NS::StringEncoding::UTF8StringEncoding)));
	pDesc->setFragmentFunction(lib->newFunction(NS::String::string("fs_main", NS::StringEncoding::UTF8StringEncoding)));
    pDesc->colorAttachments()->object(0)->setPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB );

	pipelineState = device->newRenderPipelineState(pDesc, &err);
	log_error( "%s", err->localizedDescription()->utf8String() );
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

	encoder->setRenderPipelineState(pipelineState);
	encoder->setVertexBuffer(vb, 0, 0);
	encoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::Integer(0), NS::Integer(3));

	encoder->endEncoding();

	buffer->presentDrawable(surface);
	buffer->commit();
}

int main(int argc, char* argv[])
{
	srand(time(NULL));

	// Keeping game running at 60 fps
	const float MS = 16.6666666666667f;
	double last = SDL_GetTicks64();
	double lag = 0.0;
	double current = 0;

	window.setDefault();
	window.create();

	view = SDL_Metal_CreateView(window.pWindow);
	device = MTL::CreateSystemDefaultDevice();
	assign_device(SDL_Metal_GetLayer(view), device);
	queue = device->newCommandQueue();


	init();


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
