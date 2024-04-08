#pragma once

#include <DirectXMath.h>
#include "CommonHeader.h"
#include "Component.h"
#include "Transform.h"
#include "GraphicsSystem.h"

namespace physx
{
	class PxShape;
}

namespace KunrealEngine
{
	class _DECLSPEC BoxCollider : public Component
	{
		friend class PhysicsSystem;
		friend class GameObject;
	private:
		BoxCollider();
	public:
		~BoxCollider();

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
		Transform* _transform;

		// public으로 뺄까..
		// 움직이는 물체인지 가만히 있는 물체인지		// default는 false->움직이는 물체
		bool _isStatic;

		// collider 크기
		DirectX::XMFLOAT3 _boxSize;

		// PhysX에 넘겨줄 Transform
		DirectX::XMFLOAT3 _position;

		// physX에 넘겨줄 Quaternion
		DirectX::XMFLOAT4 _quaternion;

		// 오브젝트의 Transform으로부터 얼마나 차이가 날 것인지
		DirectX::XMFLOAT3 _offset;

		// wireframe으로 collider의 크기를 표현해줌
		GInterface::GraphicsDebug* _debugObject;

		// 충돌했는지
		bool _isCollided;

		// 어떤 오브젝트와 충돌했는지
		GameObject* _targetObj;

		// physx에서 가지고 있는 collider의 shape변수
		physx::PxShape* _shape;

	public:
		// 충돌여부 반환
		bool IsCollided();

		// 충돌한 대상의 오브젝트
		GameObject* GetTargetObject();

		// 오브젝트의 Transform으로부터 얼마나 차이가 날 것인지 설정
		void SetOffset(float x, float y, float z);

		// offset 수치를 반환	// default는 0 0 0
		DirectX::XMFLOAT3 GetOffset();

		// Collider의 박스 크기를 설정
		void SetBoxSize(float x, float y, float z);

		// Collider의 박스 크기를 반환	// default는 1 1 1
		DirectX::XMFLOAT3 GetBoxSize();

		// Collider의 포지션 반환
		DirectX::XMFLOAT3 GetColliderPos();

		// Collider의 쿼터니언 반환
		DirectX::XMFLOAT4 GetColliderQuat();

		// Debug Object 설정
		void SetDebugMeshData();

	public:
		// 움직이지 않는 물체로 설정
		void SetStatic();

		// 움직이는 물체로 설정
		void SetDynamic();

		/// Mesh Render없이 콜라이더를 특정 본에 붙이기 위한 Test
	public:
		// 부모, 부모의 본을 설정하면 그 본을 부모로 두고 따라 다니게됨
		void SetTransform(GameObject* renderable, std::string boneName);
		// 부모가 있다면 분리하여 부모를 갖지 않도록 함
		void SeperateFromParent();

	private:
		void CalculateParentBone();

	private:
		GameObject* _parentObject;
		std::string _parentBoneName;
	};
}


