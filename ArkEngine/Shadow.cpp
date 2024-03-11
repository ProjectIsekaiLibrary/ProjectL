#include "Shadow.h"
#include <wrl/client.h>

Shadow::Shadow(ID3D11Device* device, UINT width, UINT height)
	:_width(width), _height(height), _depthMapSRV(0), _depthMapDSV(0)
{
	// 그림자 맵의 크기와 일치하는 뷰포트
	_viewport.TopLeftX = 0.0f;
	_viewport.TopLeftY = 0.0f;
	_viewport.Width = static_cast<float>(width);
	_viewport.Height = static_cast<float>(height);
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;

	// 깊이 스텐실 뷰는 비트들을 DXGI_FORMAT_D24_UNORM_S8_UINT로 해석하는 반면
	// 쉐이더 자원 뷰는 비트들을 DXGI_FORMAT_R24_UNORM_X8_TYPELESS로 해석할 
	// 것이므로 무형식 텍스쳐를 사용한다
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = _width;
	texDesc.Height = _height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* depthMap = 0;
	HRESULT(device->CreateTexture2D(&texDesc, 0, &depthMap));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	HRESULT(device->CreateDepthStencilView(depthMap, &dsvDesc, &_depthMapDSV));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	HRESULT(device->CreateShaderResourceView(depthMap, &srvDesc, &_depthMapSRV));

	//ReleaseCOM(depthMap);
}

Shadow::~Shadow()
{

}

ID3D11ShaderResourceView* Shadow::DepthMapSRV()
{
	return _depthMapSRV;
}

void Shadow::BindDsvAndSetNullRenderTarget(ID3D11DeviceContext* deviceContext)
{
	deviceContext->RSSetViewports(1, &_viewport);

	// 깊이 버퍼에만 장면을 그릴 것이므로 렌더 대상을 null 값으로 설정한다.
	// null 랜더 대상을 설정하면 색상 쓰기가 비활성화된다.
	ID3D11RenderTargetView* renderTarget[1] = { 0 };
	deviceContext->OMSetRenderTargets(1, renderTarget, _depthMapDSV);

	deviceContext->ClearDepthStencilView(_depthMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
