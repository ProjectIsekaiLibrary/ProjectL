#pragma once
#include <DirectXMath.h>
#include "CommonHeader.h"

namespace physx
{
	class PxShape;
}

namespace KunrealEngine 
{
	class GameObject;
	class Transform;

	class _DECLSPEC Collider
	{
		friend class PhysicsSystem;
	public:
		Collider();
		virtual ~Collider();

	private:

	protected:
		bool _isCollided;				// �浹�ߴ���
		Transform* _transform;			// collider�� transform ����
		GameObject* _ownerObj;			// collider ������Ʈ�� ������ �ִ� ������Ʈ
		GameObject* _targetObj;			// � ������Ʈ�� �浹�ߴ���
		physx::PxShape* _shape;			// physx���� ������ �ִ� collider�� shape
		
		DirectX::XMFLOAT3 _position;	// PhysX�� �Ѱ��� ��ġ��
		DirectX::XMFLOAT3 _scale;		// physx�� �Ѱ��� ũ��	// transform�� ������
		DirectX::XMFLOAT4 _quaternion;	// physX�� �Ѱ��� Quaternion
		DirectX::XMFLOAT3 _offset;		// ������Ʈ�� Transform���κ��� �󸶳� ���̰� �� ������
	
	public:
		// �浹 ���� ��ȯ
		bool IsCollided();

		// collider ������Ʈ�� ������Ʈ ��ȯ
		GameObject* GetOwnerObject();

		// �浹�� ��� ������Ʈ ��ȯ
		GameObject* GetTargetObject();

		// ������Ʈ�� Transform���κ��� �󸶳� ���̰� �� ������ ����
		void SetOffset(float x, float y, float z);
		void SetOffset(const DirectX::XMFLOAT3& offset);

		// offset ��ġ�� ��ȯ		// �⺻���� 0 0 0
		DirectX::XMFLOAT3 GetOffset();

		// collider�� ��ġ�� ��ȯ
		DirectX::XMFLOAT3 GetColliderPos();

		// collider�� ũ�� ��ȯ
		DirectX::XMFLOAT3 GetColliderScale();

		// collider�� ���ʹϾ� ��ȯ
		DirectX::XMFLOAT4 GetColliderQuaternion();

	public:
		/// collider �������� �ٸ��� �����ؾ��ϴ� �����Լ���
		virtual void SetColliderScale(float x, float y, float z) abstract;
		virtual void SetColliderScale(const DirectX::XMFLOAT3& scale) abstract;
	};
}
