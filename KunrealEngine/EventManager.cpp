#include "GameObject.h"
#include "Player.h"
#include "PlayerAbility.h"
#include "PlayerMove.h"
#include "Kamen.h"
#include "ToolBox.h"
#include "EventManager.h"
#include "Camera.h"
#include "ToolBox.h"

KunrealEngine::EventManager::EventManager()
	:_player(nullptr), _boss(nullptr), _playerComp(nullptr), _bossComp(nullptr), _playerAbill(nullptr),
	_eventStart(false), _mainCamera(nullptr), _insideSafeCount(0), _iscamfollow(true)
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
	 
	if (_iscamfollow && _mainCamera != nullptr && _player != nullptr)
	{
		DirectX::XMFLOAT3 plpos = _player->GetComponent<Transform>()->GetPosition();
		static DirectX::XMFLOAT3 capos = _mainCamera->GetComponent<Transform>()->GetPosition();

		plpos.x += capos.x + _camshakex;
		plpos.y += capos.y;
		plpos.z += capos.z + _camshakez;

		_mainCamera->GetComponent<Transform>()->SetPosition(plpos);
	}
}

void KunrealEngine::EventManager::SetCamera(std::string name)
{
	_mainCamera = SceneManager::GetInstance().GetCurrentScene()->GetGameObject(name);
}

std::vector<DirectX::XMFLOAT2> KunrealEngine::EventManager::CamShake(float radius, int numPoints)
{
	// 원형 카메라 회전
	// 원의 테두리에서 정점을 numpoint 만큼 추려낸 후, 그 점들을 반환.
	// 원들의 위치를 순차적으로 적용한다면 카메라는 원을 그리며 흔들린다.(목표)
#define M_PI       3.14159265358979323846
	std::vector<DirectX::XMFLOAT2> result;

	for (int i = 0; i < numPoints; ++i) {
		float random = ToolBox::GetRandomFloat(0.0f, radius * M_PI);
		float theta = random;
		float x = radius * std::cos(theta);
		float y = radius * std::sin(theta);
		result.push_back(DirectX::XMFLOAT2(x, y));
	}

	return result;
}

std::vector<float> KunrealEngine::EventManager::CamShakeLinear(float sigma, int numPoints)
{
	// 균등감소 카메라 흔들기
	// sigma값 범위 만큼 카메라를 흔든다. 이 sigma 값은 점점 작아진다.
	// 이때의 좌우로 진동 하는 횟수는 numpoint 값이다.
	std::vector<float> result;
	float cur_sigma = sigma;

	for (float a = 0; a < numPoints ; a++)
	{
		// 현재 진동 강도를 리스트에 추가 (양수)
		result.push_back(static_cast<int>(cur_sigma));
		// 현재 진동 강도를 리스트에 추가 (음수)
		result.push_back(static_cast<int>(-cur_sigma));
		// 진동 강도 감소
		cur_sigma -= sigma /numPoints;
	}
	result.push_back(static_cast<int>(0));

	return result;
}

void KunrealEngine::EventManager::CamShake()
{
	// 무작위 카메라 흔들기. 
	// 그냥 지정된 범위 내 랜덤값을 때려박는다.
	_camshakex = ToolBox::GetRandomFloat(0.0f, 2.0f);
	_camshakez = ToolBox::GetRandomFloat(0.0f, 2.0f);
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
		else if (nowPattern->_colliderType == BossPattern::eColliderType::eBox)
		{
			auto subObjectList = nowPattern->_subObject;

			for (int i = 0; i < subObjectList.size(); i++)
			{
				auto collider = subObjectList[i]->GetComponent<BoxCollider>();
					
				if (collider != nullptr && !nowPattern->_isColliderActive[i])
				{
					collider->SetActive(false);
				}

				// 콜라이더가 켜졌지만 맞지 않은 상태
				if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
				{
					collider->SetActive(true);

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{
						// 보스와 서브 오브젝트 사이의 디렉션으로 넘어뜨림
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

		else if (nowPattern->_colliderType == BossPattern::eColliderType::eCylinder)
		{
			auto subObjectList = nowPattern->_subObject;

			for (int i = 0; i < subObjectList.size(); i++)
			{
				auto collider = subObjectList[i]->GetComponent<CylinderCollider>();

				if (collider != nullptr && !nowPattern->_isColliderActive[i])
				{
					collider->SetActive(false);
				}

				// 콜라이더가 켜졌지만 맞지 않은 상태
				if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
				{
					collider->SetActive(true);

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{
						auto colliderDirVec = SetWarningAttackDirection(subObjectList[i]);
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

		// 박스 콜라이더일 경우
		else if (nowPattern->_colliderType == BossPattern::eColliderType::eBox)
		{
			auto subObjectList = nowPattern->_subObject;

			for (int i = 0; i < subObjectList.size(); i++)
			{
				auto collider = subObjectList[i]->GetComponent<BoxCollider>();

				if (collider != nullptr && !nowPattern->_isColliderActive[i])
				{
					collider->SetActive(false);
				}

				// 콜라이더가 켜졌지만 맞지 않은 상태
				if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
				{
					collider->SetActive(true);

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{
						// 소드의 방향 벡터로 넘어뜨림
						_playerComp->CalculateSweep(_bossComp->GetSwordDirection());

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

		// 추후 수정 예정
		else if (nowPattern->_colliderType == BossPattern::eColliderType::eCylinder)
		{
			auto subObjectList = nowPattern->_subObject;

			for (int i = 0; i < subObjectList.size(); i++)
			{
				auto collider = subObjectList[i]->GetComponent<CylinderCollider>();

				if (collider != nullptr && !nowPattern->_isColliderActive[i])
				{
					collider->SetActive(false);
				}

				// 콜라이더가 켜졌지만 맞지 않은 상태
				if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
				{
					collider->SetActive(true);

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{
						auto colliderDirVec = SetWarningAttackDirection(subObjectList[i]);
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

		else if (nowPattern->_colliderType == BossPattern::eColliderType::eCircleSafe)
		{
			auto subObjectList = nowPattern->_subObject;

			for (int i = 0; i < subObjectList.size(); i++)
			{
				auto collider = subObjectList[i]->GetComponent<CylinderCollider>();

				if (collider != nullptr && !nowPattern->_isColliderActive[i])
				{
					collider->SetActive(false);

					_insideSafeCount = 0;
				}

				// 콜라이더가 켜졌지만 맞지 않은 상태
				if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
				{
					collider->SetActive(true);

					collider->FixedUpdate();

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{
						
					}
					else
					{
						_insideSafeCount++;

						if (_insideSafeCount > 1)
						{
							// 소드와 서브오젝트의 콜라이더
							auto colliderDirVec = SetWarningAttackDirection(subObjectList[i]);

							_playerComp->CalculateSweep(colliderDirVec);

							auto damage = nowPattern->_damage;

							// 플레이어의 hp에서 패턴의 데미지만큼 차감시킴
							_playerComp->GetPlayerData()._hp -= damage;
							_playerComp->SetHitState(static_cast<int> (nowPattern->_attackState));

							nowPattern->_isColliderHit[i] = true;
						}
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

		else if (nowPattern->_colliderType == BossPattern::eColliderType::eDonut)
		{
			auto subObjectList = nowPattern->_subObject;
		
			for (int i = 0; i < subObjectList.size(); i++)
			{
				auto collider = subObjectList[i]->GetComponent<CylinderCollider>();

				if (collider!=nullptr && !nowPattern->_isColliderActive[i])
				{
					collider->SetActive(false);
				}

				// 콜라이더가 켜졌지만 맞지 않은 상태
				if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
				{
					collider->SetActive(true);

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{

						auto a = _bossComp->GetSwordPos();


						auto colliderDirVec = SetWarningAttackDirection(subObjectList[i]);
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
						//nowPattern->_isColliderHit[i] = true;
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
	colliderPos.y = playerPos.y;

	auto colliderDirVec = ToolBox::GetDirectionVec(colliderPos, playerPos);

	return colliderDirVec;
}

const DirectX::XMVECTOR& KunrealEngine::EventManager::SetBossAttackDirection(GameObject* subObject)
{
	auto colliderPos = subObject->GetComponent<Transform>()->GetPosition();
	auto bossPos = _boss->GetComponent<Transform>()->GetPosition();
	auto playerPos = _player->GetComponent<Transform>()->GetPosition();

	colliderPos.y = playerPos.y;
	bossPos.y = playerPos.y;

	auto colliderDirVec = ToolBox::GetDirectionVec(bossPos, colliderPos);

	return colliderDirVec;
}
