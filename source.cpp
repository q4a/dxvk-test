#include <iostream>

#ifdef _WIN32
#include <d3dcompiler.h>
#include <dxgi.h>
#pragma comment (lib, "DXGI.lib")
#pragma comment (lib, "d3dcompiler.lib")
#else // _WIN32

#if defined(DISABLE_DXVK)
#include <vkd3d_utils.h>
#elif defined(USE_DXVK_THEN_VKD3D_HEADERS)
#include <dxgi.h>
#define __VKD3D_WINDOWS_H
#define __VKD3D_UNKNOWN_H
#define __vkd3d_dxgibase_h__
#include <vkd3d_utils.h>
#include <SDL2/SDL.h>
#elif defined(USE_VKD3D_THEN_DXVK_HEADERS)
#include <vkd3d_utils.h>
#include <vkd3d_dxgi.h>
#define __dxgi_h__
#ifndef WINBOOL
typedef BOOL WINBOOL;
#endif
#ifndef LPSTR
typedef char* LPSTR;
typedef const char* LPCSTR;
#endif
#include <d3d11.h>
//#define __dxgitype_h__
//#include <dxgi.h>
//#include <SDL2/SDL.h>
#else // use only DXVK headers
#include <d3dcompiler.h>
#include <dxgi.h>
#include <SDL2/SDL.h>
#endif

#endif // _WIN32

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
