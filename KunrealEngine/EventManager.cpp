#include "GameObject.h"
#include "Player.h"
#include "PlayerAbility.h"
#include "PlayerMove.h"
#include "Kamen.h"
#include "ToolBox.h"
#include "EventManager.h"
#include "Camera.h"
#include "MeshCollider.h"
#include "ToolBox.h"
#include "Navigation.h"

KunrealEngine::EventManager::EventManager()
	:_player(nullptr), _boss(nullptr), _playerComp(nullptr), _bossComp(nullptr), _playerAbill(nullptr),
	_eventStart(false), _mainCamera(nullptr), _insideSafeCount(0), _iscamfollow(true)
	, _fadeObjectTitle(nullptr), _fadeObjectMain(nullptr)
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
		CalculateDamageToPlayer();

		CalculateDamageToPlayer2();
	}

	if (_player != nullptr)
	{
		if (InputSystem::GetInstance()->KeyDown(KEY::SPACE) && _player->GetComponent<PlayerMove>()->GetisDashed())
		{
			_CoroutineIs(DashCamMove)
			{
				// 캠 팔로를 끈다.
				_iscamfollow = false;
				// 시간을 0.1초 지연시킨다
				// 플레이어의 타겟 포지션을 받아온다.
				DirectX::XMFLOAT3 targetpos = _player->GetComponent<PlayerMove>()->GetTargetPosition();
				///GetCurrentScene()->CreateObject("targetpos")->GetComponent<Transform>()->SetPosition(targetpos);
				// 플레이어의 위치를 받아온다.
				DirectX::XMFLOAT3 playerpos = _player->GetComponent<Transform>()->GetPosition();
				///GetCurrentScene()->CreateObject("playerpos")->GetComponent<Transform>()->SetPosition(playerpos);
				Waitforsecond(0.1f);

				// 카메라의 위치를 받아온다
				DirectX::XMFLOAT3 campos = _mainCamera->GetComponent<Transform>()->GetPosition();
				DirectX::XMFLOAT3 fixedcampos = campos; // 선형보간을 위한 복사본이다.
				// 플레이어의 위치와 타겟 포지션의 위치를 빼서 이동할 벡터를 구한다.
				DirectX::XMFLOAT3 movepos = ToolBox::XMFLOAT3Subtract(targetpos, playerpos);
				// 플레이어의 위치와 카메라의 위치를 빼서 카메라와의 거리차이를 구한다.
				DirectX::XMFLOAT3 movevec = ToolBox::XMFLOAT3Subtract(playerpos, campos);
				// 이동할 벡터를 카메라의 위치에 더해 타겟포지션을 구한다.
				DirectX::XMFLOAT3 camTarget = ToolBox::XMFLOAT3Add(campos, movepos);
				camTarget.y = campos.y;

				// 다음은 카메라를 더 앞으로 빼야한다. 그러려면 일단 방향벡터를 얻어야한다.
				DirectX::XMFLOAT3 basicvec(0, 0, 0);	// 방향벡터를 구하기 위한 원점
				DirectX::XMFLOAT3 dirvec;				// 방향벡터를 담을 XMFLOAT3
				// 방향벡터를 구해서 담는다.
				DirectX::XMStoreFloat3(&dirvec, ToolBox::GetDirectionVec(basicvec, movepos));
				// 앞으로 얼마나 더 나아갈 것인지?
				float range = 3.0f;
				// 얼마나 더 나아갈지를 계산한다.
				dirvec = DirectX::XMFLOAT3(dirvec.x * range, dirvec.y * range, dirvec.z * range);
				// 그리고 그걸 camtarget에 더해서 목표지점을 만들어 준다.
				DirectX::XMFLOAT3 camoverTarget = DirectX::XMFLOAT3 \
					(dirvec.x + camTarget.x, dirvec.y + camTarget.y, dirvec.z + camTarget.z);

				// 거리계산시 오차
				float distancevalue = 0.1f;

				// 선형보간시 일정하게 감소하는 t값
				float t = 0.0f;
				//while문을 돌리자 조건은 카메라와 타겟 위치사이의 거리
				while (ToolBox::GetDistance(camoverTarget, campos) > distancevalue)
				{
					// t값을 더해준다.
					t += 0.1;
					// for문 조건 연산을 위해 campos를 받아온다.
					campos = _mainCamera->GetComponent<Transform>()->GetPosition();
					// 카메라부터 타겟 포지션까지의 거리를 0~1로 두고 선형보간을 통해 한프레임마다의 위치를 구한다.
					DirectX::XMFLOAT3 result = ToolBox::Lerp(fixedcampos, camoverTarget, t);
					// 이제 카메라를 거기로 옮기자
					_mainCamera->GetComponent<Transform>()->SetPosition(result);

					Return_null; // 이번 루프를 종료
				}

				// 이번엔 역방향으로!
				t = 0.0f;
				while (t > 1.0f)
				{
					// t값을 더해준다.
					t += 0.1;
					// for문 조건 연산을 위해 campos를 받아온다.
					campos = _mainCamera->GetComponent<Transform>()->GetPosition();
					// 카메라부터 타겟 포지션까지의 거리를 0~1로 두고 선형보간을 통해 한프레임마다의 위치를 구한다.
					// 이거 때문에 camTarget을 살려뒀지
					DirectX::XMFLOAT3 result = ToolBox::Lerp(camoverTarget, camTarget, t);
					// 이제 카메라를 거기로 옮기자
					_mainCamera->GetComponent<Transform>()->SetPosition(result);

					Return_null; // 이번 루프를 종료
				}
				// 다 끝났으니 카메라추적을 다시 켠다.
				_iscamfollow = true;
			};
			Startcoroutine(DashCamMove);
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
	_mainCamera->GetComponent<Camera>()->SetMainCamera();
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

	for (float a = 0; a < numPoints; a++)
	{
		// 현재 진동 강도를 리스트에 추가 (양수)
		result.push_back(static_cast<int>(cur_sigma));
		// 현재 진동 강도를 리스트에 추가 (음수)
		result.push_back(static_cast<int>(-cur_sigma));
		// 진동 강도 감소
		cur_sigma -= sigma / numPoints;
	}
	result.push_back(static_cast<int>(0));

	return result;
}

void KunrealEngine::EventManager::CamShake(float harder)
{
	// 무작위 카메라 흔들기. 
	// 그냥 지정된 범위 내 랜덤값을 때려박는다.
	_camshakex = ToolBox::GetRandomFloat(0.0f, harder);
	_camshakez = ToolBox::GetRandomFloat(0.0f, harder);
}


void KunrealEngine::EventManager::CreateFadeObject()
{
	this->_fadeObjectTitle = SceneManager::GetInstance().GetScene("Title")->CreateObject("FadeObject");
	//this->_fadeObjectTitle->_autoAwake = true;
	this->_fadeObjectTitle->AddComponent<ImageRenderer>();
	this->_fadeObjectTitle->GetComponent<ImageRenderer>()->SetImage("ui/blackBackground.png");
	this->_fadeObjectTitle->GetComponent<ImageRenderer>()->SetAlpha(0.0f);
	this->_fadeObjectTitle->SetActive(false);

	this->_fadeObjectMain = SceneManager::GetInstance().GetScene("Main")->CreateObject("FadeObject");
	//this->_fadeObjectMain->_autoAwake = true;
	this->_fadeObjectMain->AddComponent<ImageRenderer>();
	this->_fadeObjectMain->GetComponent<ImageRenderer>()->SetImage("ui/blackBackground.png");
	this->_fadeObjectMain->GetComponent<ImageRenderer>()->SetAlpha(0.0f);
	this->_fadeObjectMain->SetActive(false);
}

//void KunrealEngine::EventManager::CalculateDamageToBoss()
//{
//	auto& bossInfo = _bossComp->GetBossInfo();
//
//	auto damage = _playerAbill->GetDamage();
//
//	if (damage > 0)
//	{
//		auto finalDamage = damage * (100.0f / 100 + bossInfo._armor);
//
//		_bossComp->GetBossInfo()._hp -= finalDamage;
//	}
//}


void KunrealEngine::EventManager::CalculateDamageToBoss(Ability* abil)
{
	auto damage = abil->GetDamage() * _playerComp->GetPlayerData()._spellPower;

	float finalDamage = 1 / _bossComp->GetBossInfo()._armor;

	this->_bossComp->GetBossInfo()._hp -= damage * (1 / _bossComp->GetBossInfo()._armor);
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
					collider->FixedUpdate();

					collider->SetActive(true);

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{
						// 보스와 서브 오브젝트 사이의 디렉션으로 넘어뜨림
						if (nowPattern->_attackState == BossPattern::eAttackState::ePush)
						{
							if (subObjectList[i]->GetObjectName().find("Ego") == std::string::npos)
							{
								auto colliderDirVec = SetBossAttackDirection(subObjectList[i]);

								_playerComp->CalculateSweep(colliderDirVec);
							}
							else
							{
								auto colliderDirVec = SetEgoAttackDirection(subObjectList[i]);

								_playerComp->CalculateSweep(colliderDirVec);
							}
						}

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
					collider->FixedUpdate();

					collider->SetActive(true);

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{
						if (nowPattern->_attackState == BossPattern::eAttackState::ePush)
						{
							auto colliderDirVec = SetWarningAttackDirection(subObjectList[i]);
							_playerComp->CalculateSweep(colliderDirVec);
						}

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


		else if (nowPattern->_colliderType == BossPattern::eColliderType::eSwordMultiple)
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
					collider->FixedUpdate();

					collider->SetActive(true);

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{
						auto colliderDirVec = _bossComp->GetMultipleSwordDirVec(i);
						colliderDirVec.y = 0.0f;
						_playerComp->CalculateSweep(colliderDirVec);

						auto damage = nowPattern->_damage;

						// 플레이어의 hp에서 패턴의 데미지만큼 차감시킴
						_playerComp->GetPlayerData()._hp -= damage;
						_playerComp->SetHitState(static_cast<int> (nowPattern->_attackState));


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


		else if (nowPattern->_colliderType == BossPattern::eColliderType::eSwordDirection)
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
					collider->FixedUpdate();

					collider->SetActive(true);

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{
						if (nowPattern->_attackState == BossPattern::eAttackState::ePush)
						{
							auto playerPos = _player->GetComponent<Transform>()->GetPosition();
							auto bossPos = _boss->GetComponent<Transform>()->GetPosition();

							auto colliderDirVec = ToolBox::GetDirectionVec(bossPos, playerPos);

							colliderDirVec.m128_f32[1] = 0.0f;

							_playerComp->CalculateSweep(colliderDirVec);
						}

						auto damage = nowPattern->_damage;

						// 플레이어의 hp에서 패턴의 데미지만큼 차감시킴
						_playerComp->GetPlayerData()._hp -= damage;
						_playerComp->SetHitState(static_cast<int> (nowPattern->_attackState));


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

		else if (nowPattern->_colliderType == BossPattern::eColliderType::eSwordColliderWithMeshCollider)
		{
			auto subObjectList = nowPattern->_subObject;

			for (int i = 0; i < subObjectList.size(); i++)
			{
				auto collider = subObjectList[i]->GetComponent<MeshCollider>();

				if (collider == nullptr)
				{
					auto collider = subObjectList[i]->GetComponent<BoxCollider>();

					if (collider != nullptr && !nowPattern->_isColliderActive[i])
					{
						collider->SetActive(false);
					}

					// 콜라이더가 켜졌지만 맞지 않은 상태
					if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
					{
						collider->FixedUpdate();

						collider->SetActive(true);

						if (collider->IsCollided() && collider->GetTargetObject() == _player)
						{
							auto playerPos = _player->GetComponent<Transform>()->GetPosition();
							auto bossPos = _boss->GetComponent<Transform>()->GetPosition();

							auto colliderDirVec = ToolBox::GetDirectionVec(bossPos, playerPos);

							colliderDirVec.m128_f32[1] = 0.0f;

							_playerComp->CalculateSweep(colliderDirVec);


							auto damage = nowPattern->_damage;

							// 플레이어의 hp에서 패턴의 데미지만큼 차감시킴
							_playerComp->GetPlayerData()._hp -= damage;
							_playerComp->SetHitState(static_cast<int> (nowPattern->_attackState));


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

				else
				{
					if (collider != nullptr && !nowPattern->_isColliderActive[i])
					{
						collider->SetActive(false);
					}

					// 콜라이더가 켜졌지만 맞지 않은 상태
					if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
					{
						collider->FixedUpdate();

						collider->SetActive(true);

						if (collider->IsCollided() && collider->GetTargetObject() == _player)
						{
							auto playerPos = _player->GetComponent<Transform>()->GetPosition();
							auto bossPos = _boss->GetComponent<Transform>()->GetPosition();

							auto colliderDirVec = SetBossAttackDirection(subObjectList[i]);

							_playerComp->CalculateSweep(colliderDirVec);


							auto damage = nowPattern->_damage;

							// 플레이어의 hp에서 패턴의 데미지만큼 차감시킴
							_playerComp->GetPlayerData()._hp -= damage;
							_playerComp->SetHitState(static_cast<int> (nowPattern->_attackState));


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


		else if (nowPattern->_colliderType == BossPattern::eColliderType::eSwordColliderWithCylinderCollider)
		{
			auto subObjectList = nowPattern->_subObject;

			for (int i = 0; i < subObjectList.size(); i++)
			{
				auto collider = subObjectList[i]->GetComponent<CylinderCollider>();

				if (collider == nullptr)
				{
					auto collider = subObjectList[i]->GetComponent<BoxCollider>();

					if (collider != nullptr && !nowPattern->_isColliderActive[i])
					{
						collider->SetActive(false);
					}

					// 콜라이더가 켜졌지만 맞지 않은 상태
					if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
					{
						collider->FixedUpdate();

						collider->SetActive(true);

						if (collider->IsCollided() && collider->GetTargetObject() == _player)
						{
							auto playerPos = _player->GetComponent<Transform>()->GetPosition();
							auto bossPos = _boss->GetComponent<Transform>()->GetPosition();

							auto colliderDirVec = ToolBox::GetDirectionVec(bossPos, playerPos);

							colliderDirVec.m128_f32[1] = 0.0f;

							_playerComp->CalculateSweep(colliderDirVec);


							auto damage = nowPattern->_damage;

							// 플레이어의 hp에서 패턴의 데미지만큼 차감시킴
							_playerComp->GetPlayerData()._hp -= damage;
							_playerComp->SetHitState(static_cast<int> (nowPattern->_attackState));


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

				else
				{
					if (collider != nullptr && !nowPattern->_isColliderActive[i])
					{
						collider->SetActive(false);
					}

					// 콜라이더가 켜졌지만 맞지 않은 상태
					if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
					{
						collider->FixedUpdate();

						collider->SetActive(true);

						if (collider->IsCollided() && collider->GetTargetObject() == _player)
						{

							auto playerPos = _player->GetComponent<Transform>()->GetPosition();
							auto colliderPos = subObjectList[i]->GetComponent<Transform>()->GetPosition();

							auto distance = ToolBox::GetDistance(colliderPos, playerPos);

							if (20 <= distance && distance <= 50)
							{
								return;
							}

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

							//맞았다고 체크
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
					collider->FixedUpdate();

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
					collider->FixedUpdate();

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
					collider->FixedUpdate();

					collider->SetActive(true);

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

				if (collider != nullptr && !nowPattern->_isColliderActive[i])
				{
					collider->SetActive(false);
				}

				// 콜라이더가 켜졌지만 맞지 않은 상태
				if (nowPattern->_isColliderActive[i] && !nowPattern->_isColliderHit[i])
				{
					collider->FixedUpdate();

					collider->SetActive(true);

					if (collider->IsCollided() && collider->GetTargetObject() == _player)
					{

						auto playerPos = _player->GetComponent<Transform>()->GetPosition();
						auto swordPos = _bossComp->GetSwordPos();

						auto distance = ToolBox::GetDistance(swordPos, playerPos);

						if (distance <= _bossComp->GetDonutSize())
						{
							int a = _bossComp->GetDonutSize();
							return;
						}

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

const DirectX::XMVECTOR& KunrealEngine::EventManager::SetEgoAttackDirection(GameObject* subObject)
{
	auto colliderPos = subObject->GetComponent<Transform>()->GetPosition();
	auto egoPos = _bossComp->GetEgoPos();
	auto playerPos = _player->GetComponent<Transform>()->GetPosition();

	colliderPos.y = playerPos.y;
	egoPos.y = playerPos.y;

	auto colliderDirVec = ToolBox::GetDirectionVec(egoPos, colliderPos);

	return colliderDirVec;
}

void KunrealEngine::EventManager::MoveToTitleAfterDeath()
{
	// 카메라 고정 해제
	this->_iscamfollow = false;

	// 플레이어 위치 초기화
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("TitlePlayer")->GetComponent<Transform>()->SetPosition(-156.0f, 66.0f, 0.0f);
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("TitlePlayer")->GetComponent<Transform>()->SetRotation(0.0f, 45.0f, 0.0f);
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("TitlePlayer")->GetComponent<PlayerMove>()->SetPlayerY(66.0f);
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("TitlePlayer")->GetComponent<PlayerMove>()->StopPlayer();
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("TitlePlayer")->GetComponent<BoxCollider>()->FixedUpdate();

	// 타이틀 UI 위치 초기화
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("Title_Image")->GetComponent<ImageRenderer>()->SetPosition(525.0f, 20.0f);
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("button_Start")->GetComponent<ImageRenderer>()->SetPosition(0.0f, 400.0f);

	// 빛 세팅
	Light* titleLight = SceneManager::GetInstance().GetScene("Title")->GetGameObject("DirectionalLight")->GetComponent<Light>();
	titleLight->SetDirection(1.0f, -1.0f, 0.1f);
	titleLight->SetDiffuse(0.3f, 0.3f, 0.430f, 0.3f);
	titleLight->SetAmbient(0.0f, 0.06f, 0.410f, 0.2f);
	titleLight->SetSpecular(1.0f, 1.0f, 1.0f, 1.0f);

	// 세팅 후 scene 변경
	SceneManager::GetInstance().ChangeScene("Title");

	// 전투중인 플레이어 초기화
	SceneManager::GetInstance().GetScene("Main")->GetGameObject("Player")->GetComponent<Player>()->ResetPlayerStatus();

	// 네비게이션 재설정
	Navigation::GetInstance().HandleBuild(0, "bridge_mapmesh.obj");

	// 타이틀 씬의 파티클 재활성화
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("TitleRock1")->GetComponent<Particle>()->SetActive(true);
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("TitleRock2Particle")->GetComponent<Particle>()->SetActive(true);
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("TitleRock3")->GetComponent<Particle>()->SetActive(true);
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("particlePortal1")->GetComponent<Particle>()->SetActive(true);
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("particlePortal2")->GetComponent<Particle>()->SetActive(true);
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("particlePortal3")->GetComponent<Particle>()->SetActive(true);
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("particleMoon")->GetComponent<Particle>()->SetActive(true);
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("Particle18")->GetComponent<Particle>()->SetActive(true);
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("Particle18 (1)")->GetComponent<Particle>()->SetActive(true);
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("Particle18 (2)")->GetComponent<Particle>()->SetActive(true);
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("Particle18 (3)")->GetComponent<Particle>()->SetActive(true);
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("Particle18 (4)")->GetComponent<Particle>()->SetActive(true);
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("Particle18 (5)")->GetComponent<Particle>()->SetActive(true);
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("Particle18 (6)")->GetComponent<Particle>()->SetActive(true);
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("Particle18 (7)")->GetComponent<Particle>()->SetActive(true);
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("Particle18 (8)")->GetComponent<Particle>()->SetActive(true);
	SceneManager::GetInstance().GetScene("Title")->GetGameObject("Particle18 (9)")->GetComponent<Particle>()->SetActive(true);

	// 카메라 재설정 후 fadeout fadein
	SetCamera("TitleCamera");
	Startcoroutine(fadein);
}

void KunrealEngine::EventManager::ActivateFadeOutTrigger()
{
	Startcoroutine(fadeout);
}
