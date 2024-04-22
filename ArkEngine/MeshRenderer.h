/// <summary>
/// 
/// �ν��Ͻ��� Ȱ���� �������� �ǽ��ϱ� ����
/// 
/// ����ȭ
/// </summary>

#pragma once
#include <string>
#include <vector>
#include <d3d11.h>

namespace ArkEngine
{
	class IRenderable;
	class ICamera;

	namespace ArkDX11
	{
		class ArkEffect;
		class ArkDevice;
		struct Material;
	}
}

struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectMatrixVariable;
struct ID3DX11EffectVariable;
struct ID3DX11EffectShaderResourceVariable;
struct ID3DX11EffectVectorVariable;
struct ID3DX11EffectScalarVariable;
struct ID3D11ShaderResourceView;

namespace ArkEngine
{
	class MeshRenderer
	{
	public:
		MeshRenderer(IRenderable* mesh);
		~MeshRenderer();

	public:
		void AddMeshInList(IRenderable* mesh);
		void DeleteMeshInList(IRenderable* mesh);

	public:
		void Render();

		void ShadowRender();

	public:
		void SetMainCamera(ICamera* mainCamera);

	public:
		const std::string& GetName();

	private:
		void SetEffect(IRenderable* mesh);

	private:
		void Initialize(IRenderable* mesh);

	private:
		std::string _fileName;
		std::string _effectName;

	private:
		std::vector<IRenderable*> _meshList;

	private:
		std::vector<IRenderable*> _renderList;
		std::vector<IRenderable*> _shadowList;

		// ������Ʈ���� �ٸ� ��
	private:
		std::vector<DirectX::XMFLOAT4> _colorList;
		std::vector<DirectX::XMFLOAT4X4> _worldList;
		std::vector<DirectX::XMFLOAT4X4> _worldInvList;
		std::vector<DirectX::XMFLOAT4X4> _worldViewProjList;


		// ������Ʈ���� ��ġ�ϴ� ��
	private:
		std::vector<ID3D11Buffer*> _vertexBuffer;
		std::vector<ID3D11Buffer*> _indexBuffer;
		std::vector<ArkEngine::ArkDX11::Material> _material;
		std::vector<ModelMesh*> _meshes;

	private:
		ID3DX11Effect* _effect;
		ID3DX11EffectTechnique* _tech;
		ID3DX11EffectMatrixVariable* _fxBoneTransforms;

		ID3DX11EffectMatrixVariable* _fxWorld;
		ID3DX11EffectMatrixVariable* _fxWorldInvTranspose;
		ID3DX11EffectMatrixVariable* _fxworldViewProj;
		ID3DX11EffectMatrixVariable* _fxTexTransform;
		ID3DX11EffectVariable* _fxMaterial;

		ID3DX11EffectShaderResourceVariable* _diffuseMap;
		ID3DX11EffectShaderResourceVariable* _normalMap;
		ID3DX11EffectShaderResourceVariable* _emissionMap;

		ID3DX11EffectVectorVariable* _fxColor;
		ID3DX11EffectScalarVariable* _fxCartoon;

	private:
		// ���� ���� ��������
		ArkEngine::ArkDX11::ArkDevice* _arkDevice;
		ArkEngine::ArkDX11::ArkEffect* _arkEffect;

		ICamera* _mainCamera;

		std::vector<ID3D11ShaderResourceView*> _diffuseMapSrvArray;
		std::vector<ID3D11ShaderResourceView*> _normalMapSrvArray;
		std::vector<ID3D11ShaderResourceView*> _emissionMapSrvArray;

	};
}

