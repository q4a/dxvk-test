#include <iostream>
#include <vkd3d_utils.h>
#include <vkd3d_d3dcompiler.h>

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

	ID3DBlob* stripBuffer = nullptr;
	SIZE_T stripFlags = D3DCOMPILER_STRIP_REFLECTION_DATA | D3DCOMPILER_STRIP_PRIVATE_DATA;
	//D3DStripShader(codeBufferBlob->GetBufferPointer(), codeBufferBlob->GetBufferSize(), stripFlags, &stripBuffer);

	return 0;
}
