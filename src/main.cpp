#include <iostream>
#include <SDL.h>
#include <SDL_syswm.h>
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

ID3D11Device* device;
ID3D11DeviceContext* deviceCtx;
IDXGISwapChain* swap_chain_ptr = NULL;
ID3D11RenderTargetView* render_target_view_ptr = NULL;

ID3D11VertexShader* vert;
ID3D11PixelShader* frag;

ID3D11Buffer* vertexBuffer;
ID3D11Buffer* idxBuffer;

D3D11_VIEWPORT vp;

ID3D11InputLayout* lay;

constexpr D3D11_INPUT_ELEMENT_DESC vertexInputLayoutInfo[] =
{
	{
		"TEXCOORD",
		0,
		DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
		0
	},
	{
		"TEXCOORD",
		1,
		DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
		0
	},
};

void init()
{
	// Initializing SDL2
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		log_error("Error: %s", SDL_GetError());
	}

	// Initializing window
	window.create();

	//SDL_GL_MakeCurrent(window.pWindow, maincontext);

	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(window.pWindow, &wmInfo);

	DXGI_SWAP_CHAIN_DESC swap_chain_descr = { 0 };
	swap_chain_descr.BufferDesc.RefreshRate.Numerator = 0;
	swap_chain_descr.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_descr.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swap_chain_descr.SampleDesc.Count = 1;
	swap_chain_descr.SampleDesc.Quality = 0;
	swap_chain_descr.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_descr.BufferCount = 1;
	swap_chain_descr.OutputWindow = wmInfo.info.win.window;
	swap_chain_descr.Windowed = true;

	D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swap_chain_descr,
		&swap_chain_ptr,
		&device,
		NULL,
		&deviceCtx
	);

	ID3D11Texture2D* framebuffer;
	swap_chain_ptr->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(void**)&framebuffer);

	device->CreateRenderTargetView(framebuffer, 0, &render_target_view_ptr);


	ID3DBlob* blob;
	D3DReadFileToBlob(L"test_default_hlsl5_fs.fxc", &blob);
	device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &frag);
	
	D3DReadFileToBlob(L"test_default_hlsl5_vs.fxc", &blob);
	device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &vert);


	constexpr float vertices[] =
	{
		 0.0f, 0.5f, 0.0f, 0.25f, 0.39f, 0.19f, 
		 0.5f, -0.5f, 0.0f,  0.44f, 0.75f, 0.35f, 
		-0.5f, -0.5f, 0.0f, 0.38f, 0.55f, 0.20f, 
	};

	device->CreateInputLayout(vertexInputLayoutInfo, _countof(vertexInputLayoutInfo), blob->GetBufferPointer(), blob->GetBufferSize(), &lay);

	D3D11_BUFFER_DESC bufferInfo = {};
	bufferInfo.ByteWidth = sizeof(vertices);
	bufferInfo.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
	bufferInfo.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA dat = {};
	dat.pSysMem = vertices;
	device->CreateBuffer(&bufferInfo, &dat, &vertexBuffer);


	vp.Width = (float)640;
	vp.Height = (float)480;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
}

void render()
{
	UINT stride = 6 * sizeof(float);
	UINT vertexOffset = 0;

	float background_colour[4] = { 0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 1.0f };
	deviceCtx->ClearRenderTargetView(render_target_view_ptr, background_colour);

	deviceCtx->IASetInputLayout(lay);
	deviceCtx->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &vertexOffset);
	deviceCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceCtx->RSSetViewports(1, &vp);
	deviceCtx->VSSetShader(vert, NULL, 0);
	deviceCtx->PSSetShader(frag, NULL, 0);
	deviceCtx->OMSetRenderTargets(1, &render_target_view_ptr, NULL);

	deviceCtx->Draw(3, 0);

	swap_chain_ptr->Present(0, 0);
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

		//input_update();
		//audio_update();
		render();
#if !OFFLINE
		net_update();
#endif
	}

	return 0;
}
