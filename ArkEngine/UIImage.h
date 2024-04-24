/// <summary>
/// UI를 위한
/// 피킹이 되는 이미지 오브젝트
/// 
/// 윤종화
/// </summary>

#pragma once
#include "IUIImage.h"
#include "GraphicsImage.h"

struct ID3D11Buffer;
struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectMatrixVariable;
struct ID3D11ShaderResourceView;
struct ID3DX11EffectShaderResourceVariable;

namespace ArkEngine
{
	class ICamera;

	namespace ArkDX11
	{
		class ArkDevice;
		class ArkBuffer;
		class ArkEffect;
		class Transform;

		class UIImage : public ArkEngine::IUIImage, public GInterface::GraphicsImage
		{
		public:
			UIImage(const std::string& imageName, unsigned int screenWidth, unsigned int screenHeight);
			~UIImage();

		public:
			virtual void Render(bool isFinal) override;

		public:
			virtual bool GetRenderingState() override;
			virtual void SetRenderingState(bool tf) override;

		public:
			virtual const std::string& GetImageName() override;
			// 이미지 객체의 트랜스폼 설정	
			virtual void SetTransform(DirectX::XMFLOAT4X4 matrix) override;
			// 이미지 객체의 위치값 설정 (좌상단 0,0)
			virtual void SetPosition(float x = 0.0f, float y = 0.0f) override;
			// 이미지 객체의 회전값 설정
			virtual void SetRotation(float angle) override;
			// 이미지 객체의 크기값 설정
			virtual void SetScale(float x = 1.0f, float y = 1.0f) override;

			// 이미지 객체 삭제
			virtual void Delete() override;

			// 피킹을 위한 해쉬값 가져오기
			virtual unsigned int GetHashID() override;

		private:
			void Initialize();
			
		private:
			void SetEffect();
			void BuildGeometryBuffers();
			void ConvertHashToRGBA(int hashValue);
			void SetHashValue(unsigned int index);
			void SetTexture();

		private:
			float NomalizeToScreenX(float num);
			float NomalizeToScreenY(float num);

		private:
			std::string _imageName;

		private:
			ID3DX11Effect* _effect;
			ID3DX11EffectTechnique* _tech;

		private:
			ID3DX11EffectMatrixVariable* _fxWorldViewProj;

		private:
			ID3D11Buffer* _vertexBuffer;
			ID3D11Buffer* _indexBuffer;

		private:
			ID3DX11EffectShaderResourceVariable* _diffuseMap;
			ID3D11ShaderResourceView* _diffuseMapSRV;

			ID3DX11EffectVectorVariable* _fxColor;

		private:
			ArkEngine::ArkDX11::ArkDevice* _arkDevice;
			ArkEngine::ArkDX11::ArkBuffer* _arkBuffer;
			ArkEngine::ArkDX11::ArkEffect* _arkEffect;
			ArkEngine::ArkDX11::Transform* _meshTransform;


		private:
			float _color[4];
			unsigned int _hashValue;
			unsigned int _objectIndex;

		private:
			bool _isRendering;

		private:
			int _halfScreenWidth;
			int _halfScreenHeight;

			int _imageHalfWidth;
			int _imageHalfHeight;
		};
	}
}


