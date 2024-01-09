/// <summary>
/// µð¹ö±×¿ë °´Ã¼
/// 
/// À±Á¾È­
/// </summary>

#pragma once
#include <string>
#include "IDebugObject.h"

struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectMatrixVariable;
struct ID3D11Buffer;

namespace DirectX
{
	struct XMFLOAT4X4;
}

namespace ArkEngine
{
	class ICamera;

	namespace ArkDX11
	{
		class ArkEffect;
		class ArkDevice;
		class Transform;
	}
}

namespace KunrealEngine
{
	namespace KunrealMath
	{
		struct Matrix4x4;
	}
}

namespace ArkEngine
{
	namespace ArkDX11
	{
		class DebugObject : public IDebugObject
		{
		public:
			enum class eDebugType
			{
				Box,
				Shpere,
				Capsule
			};

		public:
			DebugObject(const std::string& objectName, eDebugType type);
			DebugObject(const std::string& objectName, eDebugType type, float centerPos, float range);

			~DebugObject();


		public:
			void Initialize() override;
			void Update(ArkEngine::ICamera* p_Camera) override;
			void Render() override;
			void Finalize() override;

		public:
			virtual bool GetRenderingState() override;
			virtual void SetRenderingState(bool tf) override;

		public:
			virtual void SetTransformMatrix(DirectX::XMFLOAT4X4 matrix) override;
			virtual void SetPosition(float x = 0.0f, float y = 0.0f, float z = 0.0f) override;
			virtual void SetRotation(float x = 0.0f, float y = 0.0f, float z = 0.0f) override;
			virtual void SetScale(float x = 1.0f, float y = 1.0f, float z = 1.0f) override;

		private:
			void BuildGeomtryBuffers();
			void SetEffect();

		private:
			std::string _objectName;
			std::string _effectName;

			ID3DX11Effect* _effect;
			ID3DX11EffectTechnique* _tech;

			ID3DX11EffectMatrixVariable* _fxWorldViewProj;

			DirectX::XMFLOAT4X4 _world;
			DirectX::XMFLOAT4X4 _view;
			DirectX::XMFLOAT4X4 _proj;

			ID3D11Buffer* _vertexBuffer;
			ID3D11Buffer* _indexBuffer;

			ArkEngine::ArkDX11::ArkDevice* _arkDevice;
			ArkEngine::ArkDX11::ArkEffect* _arkEffect;

			ArkEngine::ArkDX11::Transform* _meshTransform;

		private:
			unsigned int _totalIndexCount;
			bool _isRendering;

		private:
			float _width;
			float _height;
			float _depth;
			float _minPos;

			eDebugType _type;
		};
	}
}

