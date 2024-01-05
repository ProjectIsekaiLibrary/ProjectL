#pragma once
#include "Component.h"
#include "GraphicsSystem.h"
#include "../KunrealMath/MathHeaders.h"


namespace GInterface
{
	class GraphicsRenderable;
	struct Material;
}

#pragma warning (disable: 4251)

namespace KunrealEngine
{
	class Transform;

	class _DECLSPEC MeshRenderer : public Component
	{
	public:
		MeshRenderer();
		~MeshRenderer();

	public:
		virtual void Initialize() override;
		virtual void Finalize() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		virtual void OnTriggerEnter() override;
		virtual void OnTriggerStay() override;
		virtual void OnTriggerExit() override;

		virtual void SetActive(bool active) override;

	private:
		GInterface::GraphicsRenderable* _mesh;
		Transform* _transform;

		// 그림을 그리기 위한 정보들
		// 에디터에 수치를 띄워주기 위해 데이터를 멤버로 가지고 있음
	private:
		std::string _meshFileName;
		std::string _textureName;
		std::string _normalName;
		KunrealMath::Float4 _reflect;

	public:
		// 처음 컴포넌트를 생성한 후 실행해줘야함
		// 매개변수로 mesh 파일 이름, 텍스처 이름 -> 텍스처는 default 매개변수로 nullptr 가지고 있음
		void SetMeshObject(const char* fileName, const char* textureName = nullptr);
		std::string GetMeshName();

		// 이미 mesh를 무언가 가지고 있을 때 교체하는 함수
		void ChangeMeshObject(const char* fileName);

		// 그림을 그릴 지 여부 결정
	private:
		void SetRenderingState(bool flag);

	public:
		bool GetRenderingState();

		// 텍스처
		// 처음 만들 때 같이 넣어줬으면 안해도 됌		//교체용으로도 사용 가능
		void SetDiffuseTexture(const char* textureName);
		std::string GetTextureName();

		// 노말
		void SetNormalTexture(const char* textureName);
		std::string GetNormalName();

		// 매터리얼
		void SetMaterial(GInterface::Material material);
		GInterface::Material GetMaterial();

		// 리플렉트		// 반사계수
		void SetReflect(float x, float y, float z, float w);
		KunrealMath::Float4 GetReflect();

		// _mesh가 null일때, 즉 아무것도 안 들어있을 때 false 아니면 true
		// 이 컴포넌트는 처음에 SetMeshObject를 실행해서 mesh 파일을 넣어줘야한다
		bool GetMeshStatus();
	};
}

#pragma warning (default: 4251)