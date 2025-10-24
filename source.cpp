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
#include <SDL2/SDL.h>
#include <dxgi.h>
/*
#define __VKD3D_WINDOWS_H
#define __VKD3D_UNKNOWN_H
#define __vkd3d_dxgibase_h__
#include <vkd3d_utils.h>
*/
#include <d3dcommon.h>
#define _INC_WINDOWS
#define __vkd3d_d3dcommon_h__
#define __vkd3d_dxgibase_h__
//#define __vkd3d_dxgiformat_h__
//#include <vkd3d_d3dcompiler.h>
#include <vkd3d_utils.h>
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

#define SHOW(...) std::cout << #__VA_ARGS__ << " : " << __VA_ARGS__ << std::endl

int main() {
	SHOW( std::is_same_v<int, SIZE_T> );
	SHOW( std::is_same_v<size_t, SIZE_T> );
	SHOW( std::is_same_v<size_t, unsigned long> );
	SHOW( std::is_same_v<SIZE_T, unsigned long> );
	std::cout << "sizeof(REFIID)==" << sizeof(REFIID) << std::endl;
	std::cout << "sizeof(IUnknown)==" << sizeof(IUnknown) << std::endl;
	std::cout << "sizeof(D3D_FEATURE_LEVEL)==" << sizeof(D3D_FEATURE_LEVEL) << std::endl;
	std::cout << "sizeof(SIZE_T)==" << sizeof(SIZE_T) << std::endl;
	//std::cout << "sizeof(D3D12_ROOT_SIGNATURE_DESC)==" << sizeof(D3D12_ROOT_SIGNATURE_DESC) << std::endl;
	//std::cout << "sizeof(D3D_ROOT_SIGNATURE_VERSION)==" << sizeof(D3D_ROOT_SIGNATURE_VERSION) << std::endl;
	std::cout << "sizeof(ID3DBlob)==" << sizeof(ID3DBlob) << std::endl;
	//std::cout << "sizeof(D3D12_VERSIONED_ROOT_SIGNATURE_DESC)==" << sizeof(D3D12_VERSIONED_ROOT_SIGNATURE_DESC) << std::endl;
	std::cout << "sizeof(D3D_SHADER_MACRO)==" << sizeof(D3D_SHADER_MACRO) << std::endl;
	std::cout << "sizeof(ID3DInclude)==" << sizeof(ID3DInclude) << std::endl;
	std::cout << "sizeof(UINT)==" << sizeof(UINT) << std::endl;
	std::cout << "sizeof(HANDLE)==" << sizeof(HANDLE) << std::endl;
	std::cout << "sizeof(D3D_BLOB_PART)==" << sizeof(D3D_BLOB_PART) << std::endl;
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
