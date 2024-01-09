#include <filesystem>
#include "DDSTextureLoader.h"
#include "IL/il.h"
#include "ResourceManager.h"
#include "ArkDevice.h"
#include "ArkTexture.h"

ArkEngine::ArkDX11::ArkTexture::ArkTexture(const char* textureName)
	: _diffuseMapSRV(nullptr)
{
	std::string tempString = textureName;

	if (tempString.find("dds") != std::string::npos)
	{
		CreateDDSTexture(textureName);
	}
	else
	{
		CreateTexture(textureName);
	}
}

ArkEngine::ArkDX11::ArkTexture::~ArkTexture()
{
	_diffuseMapSRV->Release();
}

void ArkEngine::ArkDX11::ArkTexture::Load(const char* fileName)
{
	ResourceManager::GetInstance()->SetResource(fileName, this);
}

void ArkEngine::ArkDX11::ArkTexture::UnLoad(const char* fileName)
{
	ResourceManager::GetInstance()->DeleteResource(fileName);
}

ID3D11ShaderResourceView* ArkEngine::ArkDX11::ArkTexture::GetDiffuseMapSRV()
{
	return _diffuseMapSRV;
}

void ArkEngine::ArkDX11::ArkTexture::CreateDDSTexture(const char* textureName)
{
	ID3D11Resource* _textureResource = nullptr;

	auto device = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkDevice>("Device")->GetDevice();

	int length = MultiByteToWideChar(CP_UTF8, 0, textureName, -1, NULL, 0);
	wchar_t* unicodeString = new wchar_t[length];
	MultiByteToWideChar(CP_UTF8, 0, textureName, -1, unicodeString, length);

	LPCWSTR tempTextureName = reinterpret_cast<LPCWSTR>(unicodeString);

	DirectX::CreateDDSTextureFromFile(device, tempTextureName, &_textureResource, &_diffuseMapSRV);

	Load(textureName);

	_textureResource->Release();
}

void ArkEngine::ArkDX11::ArkTexture::CreateTexture(const char* textureName)
{
	auto device = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkDevice>("Device")->GetDevice();

	std::string tempString = textureName;
	std::wstring wideStr = std::filesystem::path(tempString).wstring();

	const wchar_t* wcharPtr = wideStr.c_str();

	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	ILuint imageID;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	ILboolean success = ilLoadImage(wcharPtr);
	if (!success) {
		ilDeleteImages(1, &imageID);
	}

	ILubyte* imageData = ilGetData();
	ILint width = ilGetInteger(IL_IMAGE_WIDTH);
	ILint height = ilGetInteger(IL_IMAGE_HEIGHT);
	ILenum format = ilGetInteger(IL_IMAGE_FORMAT);
	ILenum type = ilGetInteger(IL_IMAGE_TYPE);

	if (width < 4096 && height < 4096)
	{
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = imageData;

		if (format == IL_RGBA)
		{
			initData.SysMemPitch = width * 4;
		}
		else if (format == IL_RGB)
		{
			initData.SysMemPitch = width * 3;
		}

		ID3D11Texture2D* texture;
		device->CreateTexture2D(&textureDesc, &initData, &texture);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;

		if (texture != nullptr)
		{
			device->CreateShaderResourceView(texture, &srvDesc, &_diffuseMapSRV);
		}

		texture->Release();

		Load(textureName);
	}
}