#pragma once
#include <DirectXMath.h>
#include "CommonHeader.h"
#include "Component.h"

/// <summary>
/// �÷��̾ �߻��� ����ü Ŭ����
/// 
/// ������ �� Collider, MeshRenderer �� �Բ� �߰���
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
		BoxCollider* _collider;			// �浹 �������ִ� ������Ʈ
		MeshRenderer* _mesh;			// ����ü�� �׷��� 3D ��ü
		DirectX::XMVECTOR _direction;	// �÷��̾ �ٶ󺸴� ���⺤��

	public:
		// ����ü ���� ����	// �Ű������� �÷��̾� �־������
		// �����ϴ� �κп��� this ������ �ǵ��� ����
		void CreateInfo(GameObject* playerObj);

		// � ��ü�� �׸� ��	// ����Ʈ�� ���̴��� ����
		void SetMeshObject(const char* meshName, const char* textureName = "", const char* normalName = "");

		// �ݶ��̴� ��ü ��ȯ
		BoxCollider* GetCollider();
			
		// ���⺤�� ��ȯ (�÷��̾��� ���⺤��)
		DirectX::XMVECTOR GetDirection();
	};
}
