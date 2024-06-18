#include "GameObject.h"
#include "Player.h"
#include "PlayerAbility.h"
#include "PlayerMove.h"
#include "Kamen.h"
#include "ToolBox.h"
#include "EventManager.h"

KunrealEngine::EventManager::EventManager()
	:_player(nullptr), _boss(nullptr), _playerComp(nullptr), _bossComp(nullptr), _playerAbill(nullptr),
	_eventStart(false)
{

}

KunrealEngine::EventManager::~EventManager()
{

}

void KunrealEngine::EventManager::Initialize()
{
	SetBossObject();

	_eventStart = true;
}

void KunrealEngine::EventManager::Release()
{
	_eventStart = false;

	_playerAbill = nullptr;
	_bossComp = nullptr;
	_playerComp = nullptr;

	_boss = nullptr;
	_player = nullptr;
}

void KunrealEngine::EventManager::Update()
{
	if (_eventStart)
	{
		CalculateDamageToBoss();

		//CalculateDamageToPlayer();

		CalculateDamageToPlayer();

		CalculateDamageToPlayer2();

		if (_bossComp->isDead())
		{
			Release();
		}
	}
}

void KunrealEngine::EventManager::CalculateDamageToBoss()
{
	auto& bossInfo = _bossComp->GetBossInfo();

	auto damage = _playerAbill->GetDamage();

	if (damage > 0)
	{
		auto finalDamage = damage * (100.0f / 100 + bossInfo._armor);

		_bossComp->GetBossInfo()._hp -= finalDamage;
	}
}

void KunrealEngine::EventManager::exCalculateDamageToPlayer()
{
	//auto nowPattern = _bossComp->GetNowPattern();
	//
	//// 보스의 하위 콜라이더를 돌면서
	//if (_bossComp->GetNowPattern() != nullptr)
	//{
	//	for (const auto& pattern : nowPattern->_patternList)
	//	{
	//		for (const auto& object : pattern->_subObject)
	//		{
	//			auto collider = object->GetComponent<BoxCollider>();
	//
	//			if (collider != nullptr)
	//			{
	//				// 콜라이더와 충돌하였고 그 대상이 플레이어라면
	//				if (collider->GetActivated())
	//				{
	//					if (nowPattern->_colliderOnCount > 0)
	//					{
	//						if (collider->IsCollided() && collider->GetTargetObject() == _player)
	//						{
	//							// 여러번 공격판정이 되는거를 막기 위해 콜라이더를 끄고
	//							collider->SetActive(false);
	//
	//							// 패턴의 최대 타격 횟수에서 하나를 감소시킴
	//							nowPattern->_colliderOnCount--;
	//
	//							// 패턴의 데미지를 가져옴
	//							auto damage = nowPattern->_damage;
	//
	//							// 플레이어의 hp에서 패턴의 데미지만큼 차감시킴
	//							_playerComp->GetPlayerData()._hp -= damage;
	//							_playerComp->SetHitState(static_cast<int> (nowPattern->_attackState));
	//
	//							// 데미지가 들어간 후 메쉬를 꺼야한다면
	//							if (object->GetComponent<MeshRenderer>() != nullptr && !nowPattern->_isRemainMesh)
	//							{
	//								// 메쉬를 꺼버림
	//								object->GetComponent<MeshRenderer>()->SetActive(false);
	//							}
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
}


void KunrealEngine::EventManager::CalculateDamageToPlayer()
{
	auto nowPattern = _bossComp->GetNowPlayingPattern();

	// 보스가 공격중일때만?
	if (nowPattern != nullptr)
	{
		// 콜라이더가 없을 경우
		if (nowPattern->_colliderType == BossPattern::eColliderType::eNone)
		{
			return;
		}

		// 박스 콜라이더나 실린더 콜라이더일 경우
		else if (nowPattern->_colliderType == BossPattern::eColliderType::eBox || 
			nowPattern->_colliderType == BossPattern::eColliderType::eCylinder)
		{
			auto subObjectList = nowPattern->_subObject;

			for (int i = 0; i < subObjectList.size(); i++)
			{
				auto collider = subObjectList[i]->GetComponent<BoxCollider>();
					
				// 콜라이더가 켜졌지만 맞지 않은 상태
				if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
				{
					collider->SetActive(true);

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{
						auto colliderDirVec = SetBossAttackDirection(subObjectList[i]);

						_playerComp->CalculateSweep(colliderDirVec);

						auto damage = nowPattern->_damage;

						// 플레이어의 hp에서 패턴의 데미지만큼 차감시킴
						_playerComp->GetPlayerData()._hp -= damage;
						_playerComp->SetHitState(static_cast<int> (nowPattern->_attackState));

						// 데미지가 들어간 후 메쉬를 꺼야한다면
						if (!nowPattern->_isRemainMesh && subObjectList[i]->GetComponent<MeshRenderer>() != nullptr)
						{
							// 메쉬를 꺼버림
							subObjectList[i]->GetComponent<MeshRenderer>()->SetActive(false);
						}

						// 맞았다고 체크
						nowPattern->_isColliderHit[i] = true;
					}
				}

				// 콜라이더가 켜지고 맞은 상태
				if (nowPattern->_isColliderActive[i] && nowPattern->_isColliderHit[i])
				{
					// 맞았으니 콜라이더 끄기
					nowPattern->_isColliderActive[i] = false;

					collider->SetActive(false);
				}
			}
		}
		else
		{
		}
	}
}


void KunrealEngine::EventManager::CalculateDamageToPlayer2()
{
	auto nowPattern = _bossComp->GetNowSpecialPattern();

	if (nowPattern != nullptr)
	{
		// 콜라이더가 없을 경우
		if (nowPattern->_colliderType == BossPattern::eColliderType::eNone)
		{
			return;
		}

		// 박스 콜라이더나 실린더 콜라이더일 경우
		else if (nowPattern->_colliderType == BossPattern::eColliderType::eBox ||
			nowPattern->_colliderType == BossPattern::eColliderType::eCylinder)
		{
			auto subObjectList = nowPattern->_subObject;

			for (int i = 0; i < subObjectList.size(); i++)
			{
				auto collider = subObjectList[i]->GetComponent<BoxCollider>();

				// 콜라이더가 켜졌지만 맞지 않은 상태
				if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
				{
					collider->SetActive(true);

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{
						auto damage = nowPattern->_damage;

						// 플레이어의 hp에서 패턴의 데미지만큼 차감시킴
						_playerComp->GetPlayerData()._hp -= damage;
						_playerComp->SetHitState(static_cast<int> (nowPattern->_attackState));

						// 데미지가 들어간 후 메쉬를 꺼야한다면
						if (!nowPattern->_isRemainMesh && subObjectList[i]->GetComponent<MeshRenderer>() != nullptr)
						{
							// 메쉬를 꺼버림
							subObjectList[i]->GetComponent<MeshRenderer>()->SetActive(false);
						}

						// 맞았다고 체크
						nowPattern->_isColliderHit[i] = true;
					}
				}

				// 콜라이더가 켜지고 맞은 상태
				if (nowPattern->_isColliderActive[i] && nowPattern->_isColliderHit[i])
				{
					// 맞았으니 콜라이더 끄기
					nowPattern->_isColliderActive[i] = false;

					collider->SetActive(false);
				}
			}
		}
		else if (nowPattern->_colliderType == BossPattern::eColliderType::eCircleSafe)
		{
			auto subObjectList = nowPattern->_subObject;

			for (int i = 0; i < subObjectList.size(); i++)
			{
				auto collider = subObjectList[i]->GetComponent<BoxCollider>();

				// 콜라이더가 켜졌지만 맞지 않은 상태
				if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
				{
					collider->SetActive(true);

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{
						int a = 5;
					}
					else
					{
						auto colliderDirVec = SetWarningAttackDirection(subObjectList[i]);

						_playerComp->CalculateSweep(colliderDirVec);
						
						auto damage = nowPattern->_damage;
						
						// 플레이어의 hp에서 패턴의 데미지만큼 차감시킴
						_playerComp->GetPlayerData()._hp -= damage;
						_playerComp->SetHitState(static_cast<int> (nowPattern->_attackState));
						
						nowPattern->_isColliderHit[i] = true;
					}
				}

				// 콜라이더가 켜지고 맞은 상태
				if (nowPattern->_isColliderActive[i] && nowPattern->_isColliderHit[i])
				{
					// 맞았으니 콜라이더 끄기
					nowPattern->_isColliderActive[i] = false;

					collider->SetActive(false);
				}
			}
		}
	}
}

void KunrealEngine::EventManager::SetBossObject()
{
	_player = SceneManager::GetInstance().GetCurrentScene()->GetObjectWithTag("Player");
	_boss = SceneManager::GetInstance().GetCurrentScene()->GetObjectWithTag("Boss");

	_playerComp = _player->GetComponent<Player>();
	_playerAbill = _player->GetComponent<PlayerAbility>();
	_bossComp = _boss->GetComponent<Kamen>();
}


const DirectX::XMVECTOR& KunrealEngine::EventManager::SetWarningAttackDirection(GameObject* subObject)
{
	auto colliderPos = subObject->GetComponent<Transform>()->GetPosition();
	auto playerPos = _player->GetComponent<Transform>()->GetPosition();

	auto colliderDirVec = ToolBox::GetDirectionVec(colliderPos, playerPos);

	return colliderDirVec;
}

const DirectX::XMVECTOR& KunrealEngine::EventManager::SetBossAttackDirection(GameObject* subObject)
{
	auto colliderPos = subObject->GetComponent<Transform>()->GetPosition();
	auto bossPos = _boss->GetComponent<Transform>()->GetPosition();

	auto colliderDirVec = ToolBox::GetDirectionVec(bossPos, colliderPos);

	return colliderDirVec;
}
