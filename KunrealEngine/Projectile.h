#pragma once
#include <DirectXMath.h>
#include "CommonHeader.h"
#include "Component.h"

/// <summary>
/// 플레이어가 발사할 투사체 클래스
/// 
/// 생성될 때 Collider, MeshRenderer 도 함께 추가됌
/// </summary>

namespace KunrealEngine
{
	class GameObject;
	class BoxCollider;
	class MeshRenderer;

	class _DECLSPEC Projectile : public Component
	{
	public:
		Projectile();
		~Projectile();

	public:
		void Initialize() override;
		void Release() override;

		void FixedUpdate() override;
		void Update() override;
		void LateUpdate() override;

		void OnTriggerEnter() override;
		void OnTriggerStay() override;
		void OnTriggerExit() override;

		void SetActive(bool active) override;

	private:
		BoxCollider* _collider;			// 충돌 감지해주는 컴포넌트
		MeshRenderer* _mesh;			// 투사체를 그려줄 3D 물체
		DirectX::XMVECTOR _direction;	// 플레이어가 바라보는 방향벡터

	public:
		// 투사체 정보 생성	// 매개변수로 플레이어 넣어줘야함
		// 생성하는 부분에선 this 넣으면 되도록 설계
		void CreateInfo(GameObject* playerObj);

		// 어떤 물체를 그릴 지	// 이펙트는 쉐이더를 통해
		void SetMeshObject(const char* meshName, const char* textureName = "", const char* normalName = "");

		// 콜라이더 객체 반환
		BoxCollider* GetCollider();
			
		// 방향벡터 반환 (플레이어의 방향벡터)
		DirectX::XMVECTOR GetDirection();
	};
}

