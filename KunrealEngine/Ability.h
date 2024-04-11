#pragma once
#include <functional>
#include "CommonHeader.h"
#include "MeshRenderer.h"
#include "ImageRenderer.h"

namespace KunrealEngine
{
	class Ability
	{
		friend class EventManager;
	public:
		Ability();
		~Ability();

		void Initialize();
		void Release();
		void Update();

		void SetActive(bool active);

	private:
		std::string _abilityName;	// ��ų �̸�

		float _damage;				// ���ط�	// �÷��̾��� damageScale�� ������
		float _stamina;				// ��� �Ҹ�
		float _staggerDamage;		// ����ȭ ���ط�
		float _cooldown;			// ���� ���ð�
		float _range;				// ��Ÿ�

		bool _isActivated;			// ��밡���Ѱ�	// ��ų â�� �ö� �ִ���
		bool _isUsable;				// ���� ���Ҵ°�
		bool _isHit;				// ������ �ǰݴ��ߴ���

		MeshRenderer* _effect;		/// ����Ʈ Ŭ������ ���� ��� �ӽ÷� ���Ⱑ �ٲ�����
		ImageRenderer* _icon;		// ��ų ������

		std::function<void()> _abilityLogic;	// ��ų ����

	public:
		// ��ų ���� ���� �Է�
		void SetTotalData(std::string name, float damage, float stamina, float sDamage, float cooldown, float range);
	
		// ��ų ������ ���� �Է�
		void SetAbilityName(std::string name);
		void SetDamage(float damage);
		void SetRequireStamina(float stamina);
		void SetStaggerDamage(float stag);
		void SetCoolDown(float cd);
		void SetRange(float range);

		// ��ų�� ��� �۵��� ���ΰ� ����
		void SetLogic(std::function<void()> logic);
	};
}
