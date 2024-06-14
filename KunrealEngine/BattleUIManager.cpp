#include "BattleUIManager.h"
#include "EventManager.h"
#include "TimeManager.h"
#include "PlayerAbility.h"

KunrealEngine::BattleUIManager::BattleUIManager()
	:_bosshpsize(5.0f), _playerhpsize(5.0f), _skillcoolsize(1.21f)
{
	_eventmanager = &KunrealEngine::EventManager::GetInstance();
}

KunrealEngine::BattleUIManager::~BattleUIManager()
{
}

void KunrealEngine::BattleUIManager::Initialize()
{
	KunrealEngine::SceneManager& scene = KunrealEngine::SceneManager::GetInstance();

	KunrealEngine::GameObject* background;			// 전투 UI 배경
	KunrealEngine::GameObject* _bosshp_barback;		// 보스 체력바 배경
	KunrealEngine::GameObject* _playerhp_barback;	// 플레이어 체력바 배경
	KunrealEngine::GameObject* _ui_skill1;		// 1번 스킬
	KunrealEngine::GameObject* _ui_skill2;		// 2번 스킬
	KunrealEngine::GameObject* _ui_skill3;		// 3번 스킬
	KunrealEngine::GameObject* _ui_skill4;		// 4번 스킬

	_battleuibox = this->GetOwner();

	background = scene.GetCurrentScene()->CreateObject("background");
	background->SetParent(_battleuibox);
	background->AddComponent<ImageRenderer>();
	background->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	background->GetComponent<ImageRenderer>()->SetPosition(460.0f, 815.0f);
	background->GetComponent<Transform>()->SetScale(5.4f, 2.5f, 1.0f);

	_ui_skill1 = scene.GetCurrentScene()->CreateObject("ui_skill1");
	_ui_skill1->SetParent(_battleuibox);
	_ui_skill1->AddComponent<ImageRenderer>();
	_ui_skill1->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	_ui_skill1->GetComponent<ImageRenderer>()->SetPosition(570.f, 910.f);
	_ui_skill1->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

	_ui_skill1_cool = scene.GetCurrentScene()->CreateObject("_ui_skill1_cool");
	_ui_skill1_cool->SetParent(_battleuibox);
	_ui_skill1_cool->AddComponent<ImageRenderer>();
	_ui_skill1_cool->GetComponent<ImageRenderer>()->SetImage("black_sheet.png");
	_ui_skill1_cool->GetComponent<ImageRenderer>()->SetPosition(570.f, 910.f);
	_ui_skill1_cool->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

	_ui_skill2 = scene.GetCurrentScene()->CreateObject("ui_skill2");
	_ui_skill2->SetParent(_battleuibox);
	_ui_skill2->AddComponent<ImageRenderer>();
	_ui_skill2->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	_ui_skill2->GetComponent<ImageRenderer>()->SetPosition(724.f, 910.f);
	_ui_skill2->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

	_ui_skill2_cool = scene.GetCurrentScene()->CreateObject("_ui_skill2_cool");
	_ui_skill2_cool->SetParent(_battleuibox);
	_ui_skill2_cool->AddComponent<ImageRenderer>();
	_ui_skill2_cool->GetComponent<ImageRenderer>()->SetImage("black_sheet.png");
	_ui_skill2_cool->GetComponent<ImageRenderer>()->SetPosition(724.f, 910.f);
	_ui_skill2_cool->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

	_ui_skill3 = scene.GetCurrentScene()->CreateObject("ui_skill3");
	_ui_skill3->SetParent(_battleuibox);
	_ui_skill3->AddComponent<ImageRenderer>();
	_ui_skill3->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	_ui_skill3->GetComponent<ImageRenderer>()->SetPosition(880.f, 910.f);
	_ui_skill3->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

	_ui_skill3_cool = scene.GetCurrentScene()->CreateObject("_ui_skill3_cool");
	_ui_skill3_cool->SetParent(_battleuibox);
	_ui_skill3_cool->AddComponent<ImageRenderer>();
	_ui_skill3_cool->GetComponent<ImageRenderer>()->SetImage("black_sheet.png");
	_ui_skill3_cool->GetComponent<ImageRenderer>()->SetPosition(880.f, 910.f);
	_ui_skill3_cool->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

	_ui_skill4 = scene.GetCurrentScene()->CreateObject("ui_skill4");
	_ui_skill4->SetParent(_battleuibox);
	_ui_skill4->AddComponent<ImageRenderer>();
	_ui_skill4->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	_ui_skill4->GetComponent<ImageRenderer>()->SetPosition(1035.f, 910.f);
	_ui_skill4->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

	_ui_skill4_cool = scene.GetCurrentScene()->CreateObject("_ui_skill4_cool");
	_ui_skill4_cool->SetParent(_battleuibox);
	_ui_skill4_cool->AddComponent<ImageRenderer>();
	_ui_skill4_cool->GetComponent<ImageRenderer>()->SetImage("black_sheet.png");
	_ui_skill4_cool->GetComponent<ImageRenderer>()->SetPosition(1035.f, 910.f);
	_ui_skill4_cool->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

	_potion = scene.GetCurrentScene()->CreateObject("ui_skill5");
	_potion->SetParent(_battleuibox);
	_potion->AddComponent<ImageRenderer>();
	_potion->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	_potion->GetComponent<ImageRenderer>()->SetPosition(1310.f, 940.f);
	_potion->GetComponent<Transform>()->SetScale(0.5f, 0.9f, 1.0f);

	_dash = scene.GetCurrentScene()->CreateObject("ui_skill6");
	_dash->SetParent(_battleuibox);
	_dash->AddComponent<ImageRenderer>();
	_dash->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	_dash->GetComponent<ImageRenderer>()->SetPosition(1200.f, 940.f);
	_dash->GetComponent<Transform>()->SetScale(0.5f, 0.9f, 1.0f);


	_playerhp_bar_downGauge = scene.GetCurrentScene()->CreateObject("playerhp_bar");
	_playerhp_bar_downGauge->SetParent(_battleuibox);
	_playerhp_bar_downGauge->AddComponent<ImageRenderer>();
	_playerhp_bar_downGauge->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	_playerhp_bar_downGauge->GetComponent<ImageRenderer>()->SetPosition(500.f, 850.f);
	_playerhp_bar_downGauge->GetComponent<Transform>()->SetScale(_playerhpsize, 0.3f, 1.0f);

	_playerhp_bar = scene.GetCurrentScene()->CreateObject("playerhp_bar");
	_playerhp_bar->SetParent(_battleuibox);
	_playerhp_bar->AddComponent<ImageRenderer>();
	_playerhp_bar->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	_playerhp_bar->GetComponent<ImageRenderer>()->SetPosition(500.f, 850.f);
	_playerhp_bar->GetComponent<Transform>()->SetScale(_playerhpsize, 0.3f, 1.0f);

	_bosshp_bar_downGauge = scene.GetCurrentScene()->CreateObject("_bosshp_bar_downGauge");
	_bosshp_bar_downGauge->SetParent(_battleuibox);
	_bosshp_bar_downGauge->AddComponent<ImageRenderer>();
	_bosshp_bar_downGauge->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	_bosshp_bar_downGauge->GetComponent<ImageRenderer>()->SetPosition(500.f, 15.f);
	_bosshp_bar_downGauge->GetComponent<Transform>()->SetScale(_bosshpsize, 0.3f, 1.0f);

	_bosshp_bar = scene.GetCurrentScene()->CreateObject("bosshp_bar");
	_bosshp_bar->SetParent(_battleuibox);
	_bosshp_bar->AddComponent<ImageRenderer>();
	_bosshp_bar->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	_bosshp_bar->GetComponent<ImageRenderer>()->SetPosition(500.f, 15.f);
	_bosshp_bar->GetComponent<Transform>()->SetScale(_bosshpsize, 0.3f, 1.0f);

	_battleuibox->SetActive(true);
	_ui_skill1_cool->SetActive(false);
	_ui_skill2_cool->SetActive(false);
	_ui_skill3_cool->SetActive(false);
	_ui_skill4_cool->SetActive(false);
}

void KunrealEngine::BattleUIManager::Release()
{
}

void KunrealEngine::BattleUIManager::FixedUpdate()
{
}

void KunrealEngine::BattleUIManager::Update()
{
	auto  abill = _eventmanager->_playerAbill;

	if (InputSystem::GetInstance()->KeyDown(KEY::UP))
	{
		int currenthp = _eventmanager->_bossComp->GetBossInfo()._hp += 10;
		SetBossHpbar();
	}

	if (InputSystem::GetInstance()->KeyDown(KEY::DOWN))
	{
		int currenthp = _eventmanager->_playerComp->GetPlayerData()._hp -= 10;
		if (_eventmanager->_bossComp->GetBossInfo()._hp <= 0)
		{
			currenthp = _eventmanager->_bossComp->GetBossInfo()._hp = 1;
		}
		SetBossHpbar();
	}

	if (InputSystem::GetInstance()->KeyDown(KEY::PGUP))
	{
		_eventmanager->_playerComp->GetPlayerData()._hp += 10;
		SetPlayerHpBar();
	}

	if (InputSystem::GetInstance()->KeyDown(KEY::PGDOWN))
	{
		_eventmanager->_playerComp->GetPlayerData()._hp -= 10;
		SetPlayerHpBar();
	}

	if (abill->_isShotDetected)
	{
		SetSkillcool1();
	}

	if (abill->_isIceDetected)
	{
		SetSkillcool2();
	}

	if (abill->_isAreaDetected)
	{
		SetSkillcool3();
	}

	if (abill->_isMeteorDetected)
	{
		SetSkillcool4();
	}
}

void KunrealEngine::BattleUIManager::LateUpdate()
{
}

void KunrealEngine::BattleUIManager::OnTriggerEnter()
{
}

void KunrealEngine::BattleUIManager::OnTriggerStay()
{
}

void KunrealEngine::BattleUIManager::OnTriggerExit()
{
}

void KunrealEngine::BattleUIManager::SetActive(bool active)
{

}

void KunrealEngine::BattleUIManager::SetBossHpbar()
{
	if (_eventmanager->_bossComp != nullptr)
	{
		float currenthp = _eventmanager->_bossComp->GetBossInfo()._hp;
		float maxhp = _eventmanager->_bossComp->GetBossInfo().GetMaxHP();
		float minhp = 0;
		float minhpbar = 0;

		booshp_targetscale = ((currenthp - minhp) / (maxhp - minhp)) * (_bosshpsize - minhpbar) + minhpbar;
		_bosshp_bar->GetComponent<Transform>()->SetScale(booshp_targetscale, 0.3f, 1.0f);
		_CoroutineIs(bossdowngauge)
		{
			//닼소식 체력 감소 연출을 어떻게 해야 할까
			// 일단 체력을 줄인다, 줄어든 체력의 앞에 다운게이지를 둔다, 다운 게이지의 스케일을 이전 체력 수치 만큼 체운다
			// 그리고 점점 스케일을 줄인다. 0이 될때까지. 
			// 줄어든 체력바의 포지션을 어떻게 잡을것이며, 이전 체력 기준으로 끝설정(스케일 조정)을 어케 할것 인가?
			// 다른 방법이 없을까
			// 일단 둘다 깐다. 빨간 체력은 바로 깐다, 노란 체력은 천천히 깐다.
			// 이러면 위치 구하고 스케일 구하고 할 필요가 없을 것 같은데 
			// 이걸로 함 해보자 

			auto  hpbarcontrol = this;
			auto  downGauge = _bosshp_bar_downGauge;	//GameObject
			float scale = downGauge->GetComponent<Transform>()->GetScale().x;
			float& targetscale = booshp_targetscale;
			float speed = 1.0f;


			while (targetscale - TimeManager::GetInstance().GetDeltaTime() * speed < scale)
			{
				downGauge->GetComponent<Transform>()->SetScale(scale, 0.3f, 1.0f);
				scale -= TimeManager::GetInstance().GetDeltaTime() * speed;
				Return_null;
			}
			targetscale = 0;
		};

		Startcoroutine(bossdowngauge);
	}
}

void KunrealEngine::BattleUIManager::SetPlayerHpBar()
{
	float currenthp = _eventmanager->_playerComp->GetPlayerData()._hp;
	//float maxhp = _eventmanager->_playerComp->GetPlayerInfo()._Maxhp;
	float maxhp = 500;
	float minhp = 0;
	float minhpbar = 0;

	playerhp_targetscale = ((currenthp - minhp) / (maxhp - minhp)) * (_bosshpsize - minhpbar) + minhpbar;
	_playerhp_bar->GetComponent<Transform>()->SetScale(playerhp_targetscale, 0.3f, 1.0f);
	_CoroutineIs(bossdowngauge)
	{
		auto  hpbarcontrol = this;
		auto  downGauge = _playerhp_bar_downGauge;	//GameObject
		float scale = downGauge->GetComponent<Transform>()->GetScale().x;
		float& targetscale = playerhp_targetscale;
		float speed = 0.5f;

		while (targetscale - TimeManager::GetInstance().GetDeltaTime() * speed < scale)
		{
			downGauge->GetComponent<Transform>()->SetScale(scale, 0.3f, 1.0f);
			scale -= TimeManager::GetInstance().GetDeltaTime() * speed;
			Return_null;
		}
		targetscale = 0;
	};

	Startcoroutine(bossdowngauge);
}

/// 1번 스킬
void KunrealEngine::BattleUIManager::SetSkillcool1()
{
	// 스킬 쿨다운을 보자.
	// 이놈은 스킬을 쓰면 까만 화면이 깔린다.
	// 쿨타임이 흐름에 따라 박스의 스케일이 줄어든다. 위든 아래든.
	// 그러면서 까만 화면이 점점 사라지고 스케일이 0이 되면 스킬쿨이 다 돌아간것. active를 끈다.
	_CoroutineIs(skillgauge1)
	{
		auto  hpbarcontrol = this;
		auto skillcool = _ui_skill1_cool;
		auto ability = _eventmanager->_playerAbill->_abilityContainer[0];

		// 스킬 쿨타임 동안 skillcool 오브젝트 활성화
		skillcool->SetActive(true);

		// 선형 보간을 위한 변수들
		// 여기서 보간 해야 할 부분은 쿨타임과 스케일이다.
		// 쿨타임은 받아온뒤 여기서 델타타임을 계속 빼준다 0이 될때까지
		// 스케일은 이 남은 쿨타임과 보간해서 현재 스케일을 구한다.

		float currenttime = ability->_cooldown;
		float maxcool = ability->_cooldown;
		float mincool = 0;

		float _coolsize = _skillcoolsize;
		float coolgauge = 0;

		float speed = 1.0f;
		
		while (0.1 <= currenttime)
		{
			currenttime -= TimeManager::GetInstance().GetDeltaTime();
			if (currenttime < 0.1)
			{
				currenttime == 0.1;
			}

			float nowscale = ((currenttime - mincool) / (maxcool - mincool)) * (_coolsize - coolgauge) + coolgauge;
			skillcool->GetComponent<Transform>()->SetScale(0.7f, nowscale, 1.0f);

			Return_null;
		}

		hpbarcontrol->_eventmanager->_playerAbill->_isShotDetected = false;
		skillcool->SetActive(false);
		skillcool->GetComponent<Transform>()->SetScale(0.7f, _skillcoolsize, 1.0f);
	};
	Startcoroutine(skillgauge1);
}

/// 2번 스킬
void KunrealEngine::BattleUIManager::SetSkillcool2()
{
	_CoroutineIs(skillgauge2)
	{
		auto  hpbarcontrol = this;
		auto skillcool = _ui_skill2_cool;
		auto ability = _eventmanager->_playerAbill->_abilityContainer[1];

		// 스킬 쿨타임 동안 skillcool 오브젝트 활성화
		skillcool->SetActive(true);

		float currenttime = ability->_cooldown;
		float maxcool = ability->_cooldown;
		float mincool = 0;

		float _coolsize = _skillcoolsize;
		float coolgauge = 0;

		float speed = 1.0f;

		while (0.1 <= currenttime)
		{
			currenttime -= TimeManager::GetInstance().GetDeltaTime();
			if (currenttime < 0.1)
			{
				currenttime == 0.1;
			}

			float nowscale = ((currenttime - mincool) / (maxcool - mincool)) * (_coolsize - coolgauge) + coolgauge;
			skillcool->GetComponent<Transform>()->SetScale(0.7f, nowscale, 1.0f);

			Return_null;
		}

		hpbarcontrol->_eventmanager->_playerAbill->_isIceDetected = false;
		skillcool->SetActive(false);
		skillcool->GetComponent<Transform>()->SetScale(0.7f, _skillcoolsize, 1.0f);
	};

	Startcoroutine(skillgauge2);
}

/// 3번 스킬
void KunrealEngine::BattleUIManager::SetSkillcool3()
{
	_CoroutineIs(skillgauge3)
	{
		auto  hpbarcontrol = this;
		auto skillcool = _ui_skill3_cool;
		auto ability = _eventmanager->_playerAbill->_abilityContainer[2];

		// 스킬 쿨타임 동안 skillcool 오브젝트 활성화
		skillcool->SetActive(true);

		float currenttime = ability->_cooldown;
		float maxcool = ability->_cooldown;
		float mincool = 0;

		float _coolsize = _skillcoolsize;
		float coolgauge = 0;

		float speed = 1.0f;

		while (0.1 <= currenttime)
		{
			currenttime -= TimeManager::GetInstance().GetDeltaTime();
			if (currenttime < 0.1)
			{
				currenttime == 0.1;
			}

			float nowscale = ((currenttime - mincool) / (maxcool - mincool)) * (_coolsize - coolgauge) + coolgauge;
			skillcool->GetComponent<Transform>()->SetScale(0.7f, nowscale, 1.0f);

			Return_null;
		}

		hpbarcontrol->_eventmanager->_playerAbill->_isAreaDetected = false;
		skillcool->SetActive(false);
		skillcool->GetComponent<Transform>()->SetScale(0.7f, _skillcoolsize, 1.0f);
	};

	Startcoroutine(skillgauge3);
}

/// 4번 스킬
void KunrealEngine::BattleUIManager::SetSkillcool4()
{
	_CoroutineIs(skillgauge4)
	{
		auto  hpbarcontrol = this;
		auto skillcool = _ui_skill4_cool;
		auto ability = _eventmanager->_playerAbill->_abilityContainer[3];

		// 스킬 쿨타임 동안 skillcool 오브젝트 활성화
		skillcool->SetActive(true);

		float currenttime = ability->_cooldown;
		float maxcool = ability->_cooldown;
		float mincool = 0;

		float _coolsize = _skillcoolsize;
		float coolgauge = 0;

		float speed = 1.0f;

		while (0.1 <= currenttime)
		{
			currenttime -= TimeManager::GetInstance().GetDeltaTime();
			if (currenttime < 0.1)
			{
				currenttime == 0.1;
			}

			float nowscale = ((currenttime - mincool) / (maxcool - mincool)) * (_coolsize - coolgauge) + coolgauge;
			skillcool->GetComponent<Transform>()->SetScale(0.7f, nowscale, 1.0f);

			Return_null;
		}

		hpbarcontrol->_eventmanager->_playerAbill->_isMeteorDetected = false;
		skillcool->SetActive(false);
		skillcool->GetComponent<Transform>()->SetScale(0.7f, _skillcoolsize, 1.0f);
	};

	Startcoroutine(skillgauge4);
}