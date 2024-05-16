#pragma once

struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectMatrixVariable;
struct ID3DX11EffectVectorVariable;
struct ID3DX11EffectScalarVariable;
struct ID3D11Buffer;

namespace DirectX
{
	struct XMFLOAT4;
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
namespace ArkEngine
{
	namespace ArkDX11
	{
		class TransparentMesh
		{
		public:
			TransparentMesh(const std::string& objectName, const std::string& textureName, float transParency, bool isCircle = false);
			~TransparentMesh();

		public:
			void Initialize();
			void Update(ArkEngine::ICamera* p_Camera);
			void Render();
			void Finalize();

		public:
			const std::string& GetName();

		private:
			void BuildGeomtryBuffers();
			void SetEffect();
			void SetTexture(const std::string& name);

		private:
			std::string _objectName;
			std::string _textureName;

			float _transParency;

			ArkEngine::ArkDX11::ArkEffect* _arkEffect;
			ID3DX11Effect* _effect;

			ArkEngine::ArkDX11::ArkDevice* _arkDevice;

			ArkEngine::ArkDX11::Transform* _meshTransform;

			ID3D11Buffer* _vertexBuffer;
			ID3D11Buffer* _indexBuffer;

			unsigned int _totalIndexCount;

			ID3DX11EffectTechnique* _tech;
			ID3DX11EffectMatrixVariable* _fxWorld;
			ID3DX11EffectMatrixVariable* _fxWorldViewProj;

			DirectX::XMFLOAT4X4 _world;
			DirectX::XMFLOAT4X4 _view;
			DirectX::XMFLOAT4X4 _proj;

			ID3DX11EffectShaderResourceVariable* _texture;
			ID3D11ShaderResourceView* _diffuseMapSRV;

			ID3DX11EffectScalarVariable* _fxTransParency;

			ID3DX11EffectScalarVariable* _fxTest;

			bool _isCircle;

			float _test;
		};
	}
}