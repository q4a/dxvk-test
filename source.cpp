#include <iostream>

#ifdef DISABLE_DXVK
#include <vkd3d_utils.h>
#else
#include <d3dcompiler.h>
#include <dxgi.h>
#ifdef _WIN32
#pragma comment (lib, "DXGI.lib")
#pragma comment (lib, "d3dcompiler.lib")
#else
#include <SDL2/SDL.h>
#endif
#endif

int main() {
	ID3DBlob* codeBufferBlob = nullptr;
	HRESULT hRet = D3DCreateBlob(5760, &codeBufferBlob);
	if (FAILED(hRet))
	{
		std::cout << "Failed to D3DCreateBlob(5760, &codeBufferBlob)" << std::endl;
		return 0;
	}
	else
	{
		std::cout << "Size of blob from D3DCreateBlob(5760, &codeBufferBlob)==" << codeBufferBlob->GetBufferSize() << std::endl;
	}

#ifndef DISABLE_DXVK
#ifndef _WIN32
	SDL_InitSubSystem(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
#endif

	IDXGIFactory* pFactory = nullptr;
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory));
	if (SUCCEEDED(hr))
	{
		std::cout << "Success on CreateDXGIFactory" << std::endl;
	}
#endif

	return 0;
}
