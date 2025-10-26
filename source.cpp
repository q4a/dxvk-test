#include <iostream>
#ifdef _WIN32
#include <d3dcompiler.h>
#include <dxgi.h>
#pragma comment (lib, "d3dcompiler.lib")
#else // _WIN32
#include <vkd3d_utils.h>
#include <vkd3d_d3dcompiler.h>
#endif // _WIN32

int main() {
	ID3DBlob* codeBufferBlob = nullptr;
	HRESULT hRet = D3DCreateBlob(5760, &codeBufferBlob);
	if (FAILED(hRet)) {
		std::cout << "Failed to D3DCreateBlob(5760, &codeBufferBlob)" << std::endl;
		return 0;
	} else {
		std::cout << "Size of blob from D3DCreateBlob(5760, &codeBufferBlob)==" << codeBufferBlob->GetBufferSize() << std::endl;
	}

	ID3DBlob* stripBuffer = nullptr;
#ifdef _WIN32
	hRet = D3DStripShader(codeBufferBlob->GetBufferPointer(), codeBufferBlob->GetBufferSize(), D3DCOMPILER_STRIP_PRIVATE_DATA, &stripBuffer);
	if (FAILED(hRet)) {
		std::cout << "As expected: failed to call D3DStripShader" << std::endl;
	}
#endif

	return 0;
}
