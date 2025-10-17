#include <iostream>
#include <dxgi.h>
#ifdef _WIN32
#pragma comment (lib, "DXGI.lib")
#else
#include <SDL2/SDL.h>
#endif

int main() {

#ifndef _WIN32
	SDL_InitSubSystem(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
#endif

	IDXGIFactory* pFactory1 = nullptr;
	HRESULT hr1 = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory1));
	if (SUCCEEDED(hr1))
	{
		std::cout << "Success on 1st CreateDXGIFactory" << std::endl;
	}

	IDXGIFactory* pFactory2 = nullptr;
	HRESULT hr2 = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory1));
	if (SUCCEEDED(hr2))
	{
		std::cout << "Success on 2nd CreateDXGIFactory" << std::endl;
	}

	return 0;
}
