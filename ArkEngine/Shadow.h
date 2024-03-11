#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>

class Shadow
{
public:
	Shadow(ID3D11Device* device, UINT width, UINT height);
	~Shadow();

	ID3D11ShaderResourceView* DepthMapSRV();
	void BindDsvAndSetNullRenderTarget(ID3D11DeviceContext* deviceContext);

private:
	Shadow(const Shadow& rhs);
	Shadow& operator = (const Shadow& rhs);

private:
	UINT _width;
	UINT _height;

	ID3D11ShaderResourceView* _depthMapSRV;
	ID3D11DepthStencilView* _depthMapDSV;

	D3D11_VIEWPORT _viewport;
};

