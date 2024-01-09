/// <summary>
/// 
/// 렌더링할 FBX 메쉬
/// 
/// 김현재
/// </summary>

#define MAX_MODEL_TRANSFORMS 96
#define MAX_MODEL_KEYFRAMES 500

#pragma once
#include <string>
#include <array>
// Buffer사용하기위해
#include "d3d11.h"
#include "CommonStruct.h"
#include "GraphicsRenderable.h"
#include "IRenderable.h"

#include "tinyxml2.h"
#include "ParsingStructs.h"

struct AnimTransform
{
	// 250개의 관절정보를 담을 수 있다
	using TransformArrayType = std::array<Matrix, MAX_MODEL_TRANSFORMS>;

	// 2차 배열
	std::array<TransformArrayType, MAX_MODEL_KEYFRAMES> _transforms;
};

struct BoneDesc
{
	Matrix transforms[96];
};

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

/// 김현재 추가
namespace ArkEngine
{
	namespace FBXLoader
	{
		class AssimpTool;
	}

	class FileUtils;
	class Utils;
}
struct ModelBone;
struct ModelMesh;
struct ModelAnimation;
struct ModelKeyframe;

enum FileMode : unsigned int;

template<typename T>
class Geometry;

class FBXAnimator;

namespace ArkEngine
{
	namespace ArkDX11
	{
		class FBXMesh : public ArkEngine::IRenderable, public GInterface::GraphicsRenderable
		{
		public:
			FBXMesh(const std::string& fileName, const std::string& textureName, bool isSolid = true);
			FBXMesh();
			~FBXMesh();

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
			IDebugObject* _debugObject;

		private:
			std::vector<unsigned int> _indexBufferSize;

		private:
			bool _isStaticMesh;
			bool _isRendering;

			bool _isSolid;

			int _myIndex;

			/// <summary>
			/// 김현재가 추가한 것들
			/// </summary>
		private:
			/// FBX 추가 김현재
			void BuildGeometryBuffersFBX(std::wstring fileName);
			void UpdateAnimationTransforms(unsigned int animationIndex,float frame);
			void BindCacheInfo();

		public:

			std::shared_ptr<ModelBone> GetBoneByIndex(unsigned int index) { return (index < 0 || index >= _bones.size() ? nullptr : _bones[index]); }
			unsigned int GetBoneCount() { return _bones.size(); }


			/// Animation 함수
			void ReadFBXAnimation(std::wstring fileName);
			unsigned int GetAnimationCount();
			std::vector<std::shared_ptr<ModelAnimation>>& GetAnimations();
			std::shared_ptr<ModelAnimation> GetAnimationByIndex(unsigned int index);
			std::shared_ptr<ModelAnimation> GetAnimationByName(std::wstring name);

			std::shared_ptr<ModelMesh> GetMeshByIndex(unsigned int index);
			std::shared_ptr<ModelMesh> GetMeshByName(std::wstring name);
		private:
			float _currentTime;
			float _animationSpeed;
			float _deltaTime;

			std::unique_ptr<ArkEngine::Utils> _utils;

			// .mesh 경로 
			std::wstring _modelPath;
			// .xml 경로
			std::wstring _texturePath;

			std::shared_ptr<ModelBone> _root;

			std::shared_ptr<ModelMesh> _modelMesh;
			std::vector<std::shared_ptr<ModelMesh>> _meshes;

			//std::shared_ptr<ModelAnimation> _anim;
		public:
			std::vector<std::shared_ptr<ModelBone>> _bones;
			std::shared_ptr<ModelKeyframe> frame;
			Matrix _invGlobal;
			std::vector<AnimTransform> _animTransforms;
		private:
			std::vector<std::shared_ptr<ModelAnimation>> _animations;
			std::shared_ptr<ModelAnimation> _anim;

			UINT _meshCount;
			//UINT _vertexNum;
			//UINT _indexNum;
			UINT _boneIndexNum;

			std::vector<Vertex> _newVertexVector;
			std::vector<unsigned int> _newIndexVector;

			std::vector<Matrix> _boneTransforms;

		};
	}
}
