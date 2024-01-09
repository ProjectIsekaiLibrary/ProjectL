/// <summary>
/// 
/// ·»´õ¸µÇÒ ¸Þ½¬
/// 
/// À±Á¾È­
/// </summary>

#pragma once
#include <string>
#include <vector>
#include "CommonStruct.h"
#include "GraphicsRenderable.h"
#include "IRenderable.h"

class ASEParser;

namespace DirectX
{
	struct XMFLOAT3;
	struct XMFLOAT4X4;
}

namespace ArkEngine
{
	class ICamera;
	class IDebugObject;

	namespace ArkDX11
	{
		class ArkEffect;
		class ArkDevice;
		class Transform;
		class Animator;

		struct Material;

		class DirectionalLight;
	}
}

namespace KunrealEngine
{
	namespace KunrealMath
	{
		struct Matrix4x4;
	}
}

struct ID3DX11Effect;
struct ID3DX11EffectTechnique;

struct ID3DX11EffectVariable;
struct ID3DX11EffectScalarVariable;
struct ID3DX11EffectMatrixVariable;
struct ID3DX11EffectVectorVariable;
struct ID3DX11EffectShaderResourceVariable;

struct ID3D11ShaderResourceView;



namespace ArkEngine
{
	namespace ArkDX11
	{
		class ASEMesh : public ArkEngine::IRenderable, public GInterface::GraphicsRenderable
		{
		public:
			ASEMesh(const std::string& fileName, const std::string& textureName);
			~ASEMesh();

		public:
			virtual void Initialize() override;
			virtual void Update(ArkEngine::ICamera* p_Camera) override;
			virtual void Render() override;
			virtual void Finalize() override;

		public:
			virtual void Delete() override;

		public:
			virtual bool GetRenderingState() override;
			virtual void SetRenderingState(bool tf) override;

		public:
			virtual void SetTransform(KunrealEngine::KunrealMath::Matrix4x4 matrix) override;
			virtual void SetPosition(float x = 0.0f, float y = 0.0f, float z = 0.0f) override;
			virtual void SetRotation(float x = 0.0f, float y = 0.0f, float z = 0.0f) override;
			virtual void SetScale(float x = 1.0f, float y = 1.0f, float z = 1.0f) override;
			virtual void SetModel(const char* fileName) override;
			virtual void SetDiffuseTexture(const char* textureName) override;
			virtual void SetNormalTexture(const char* textureName) override;
			virtual void SetAnimator() override;
			virtual const GInterface::Material GetMaterial() override;
			virtual void SetMaterial(GInterface::Material material) override;
			virtual void SetReflect(KunrealEngine::KunrealMath::Float4 reflect) override;

		public:
			virtual void PlayAnimation(float deltaTime, bool continiousPlay) override;
			virtual void StopAnimation() override;
			virtual void PlayFBXAnimation(float speed, int animIndex, bool continuousPlay) override;

		private:
			void SetEffect();
			void BuildGeometryBuffers();

		private:
			void SetBasicMaterial();

		private:
			void SetLight();

		private:
			std::string _fileName;
			std::string _effectName;
			std::string _textureName;

			DirectX::XMFLOAT4X4 _transform;

			ID3DX11Effect* _effect;
			ID3DX11EffectTechnique* _tech;

			// cbPerObject
			ID3DX11EffectMatrixVariable* _fxWorld;
			ID3DX11EffectMatrixVariable* _fxWorldInvTranspose;
			ID3DX11EffectMatrixVariable* _fxWorldViewProj;
			ID3DX11EffectMatrixVariable* _fxTexTransform;
			ID3DX11EffectVariable* _fxMaterial;

			// cbSkinned
			ID3DX11EffectMatrixVariable* _fxBoneTransforms;

			ID3DX11EffectShaderResourceVariable* _diffuseMap;

			ID3D11ShaderResourceView* _diffuseMapSRV;

			ID3DX11EffectShaderResourceVariable* _normalMap;

			ID3D11ShaderResourceView* _normalMapSRV;

			ID3DX11EffectShaderResourceVariable* _cubeMap;

			std::vector<DirectX::XMFLOAT4X4> _boneTMList;

			DirectX::XMFLOAT4X4 _world;
			DirectX::XMFLOAT4X4 _view;
			DirectX::XMFLOAT4X4 _proj;

			ID3D11Buffer* _vertexBuffer;
			ID3D11Buffer* _indexBuffer;

		private:
			ArkEngine::ArkDX11::Material _material;

			DirectX::XMFLOAT3 _eyePosW;

		private:
			ArkEngine::ArkDX11::ArkDevice* _arkDevice;
			ArkEngine::ArkDX11::ArkEffect* _arkEffect;
			UINT _totalVertexCount;
			UINT _totalIndexCount;
			
			ArkEngine::ArkDX11::Transform* _meshTransform;

		private:
			ASEParser* _aseParser;

			ArkEngine::ArkDX11::Animator* _animator;

		private:
			IDebugObject* _debugObject;

		private:
			std::vector<unsigned int> _indexBufferSize;

		private:
			bool _isStaticMesh;
			bool _isRendering;

			int _myIndex;

		};
	}
}