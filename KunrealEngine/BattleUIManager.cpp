#include "BattleUIManager.h"
#include "EventManager.h"

KunrealEngine::BattleUIManager::BattleUIManager()
	:_bosshpsize(5.0f), _playerhpsize(5.0f)
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

	_battleuibox = scene.GetCurrentScene()->CreateObject("battleuibox");

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

	_ui_skill2 = scene.GetCurrentScene()->CreateObject("ui_skill2");
	_ui_skill2->SetParent(_battleuibox);
	_ui_skill2->AddComponent<ImageRenderer>();
	_ui_skill2->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	_ui_skill2->GetComponent<ImageRenderer>()->SetPosition(724.f, 910.f);
	_ui_skill2->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

	_ui_skill3 = scene.GetCurrentScene()->CreateObject("ui_skill3");
	_ui_skill3->SetParent(_battleuibox);
	_ui_skill3->AddComponent<ImageRenderer>();
	_ui_skill3->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	_ui_skill3->GetComponent<ImageRenderer>()->SetPosition(880.f, 910.f);
	_ui_skill3->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

	_ui_skill4 = scene.GetCurrentScene()->CreateObject("ui_skill4");
	_ui_skill4->SetParent(_battleuibox);
	_ui_skill4->AddComponent<ImageRenderer>();
	_ui_skill4->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	_ui_skill4->GetComponent<ImageRenderer>()->SetPosition(1035.f, 910.f);
	_ui_skill4->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

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

	_playerhp_bar = scene.GetCurrentScene()->CreateObject("playerhp_bar");
	_playerhp_bar->SetParent(_battleuibox);
	_playerhp_bar->AddComponent<ImageRenderer>();
	_playerhp_bar->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	_playerhp_bar->GetComponent<ImageRenderer>()->SetPosition(500.f, 850.f);
	_playerhp_bar->GetComponent<Transform>()->SetScale(_playerhpsize, 0.3f, 1.0f);

	_bosshp_bar = scene.GetCurrentScene()->CreateObject("bosshp_bar");
	_bosshp_bar->SetParent(_battleuibox);
	_bosshp_bar->AddComponent<ImageRenderer>();
	_bosshp_bar->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	_bosshp_bar->GetComponent<ImageRenderer>()->SetPosition(500.f, 15.f);
	_bosshp_bar->GetComponent<Transform>()->SetScale(_bosshpsize, 0.3f, 1.0f);

	_battleuibox->SetActive(false);
}

void KunrealEngine::BattleUIManager::Release()
{
}

void KunrealEngine::BattleUIManager::FixedUpdate()
{
}

void KunrealEngine::BattleUIManager::Update()
{
	if (InputSystem::GetInstance()->KeyDown(KEY::UP))
	{
		int currenthp = _eventmanager->_bossComp->GetBossInfo()._hp += 10;
		SetBossHpbar();
	}

	if (InputSystem::GetInstance()->KeyDown(KEY::DOWN))
	{
		int currenthp = _eventmanager->_bossComp->GetBossInfo()._hp -= 10;
		SetBossHpbar();
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

		float hpsize = ((currenthp - minhp) / (maxhp - minhp)) * (_bosshpsize - minhpbar) + minhpbar;
		_bosshp_bar->GetComponent<Transform>()->SetScale(hpsize, 0.3f, 1.0f);
	}
}

void KunrealEngine::BattleUIManager::SetPlayerHpBar()
{
	//int currenthp = _eventmanager->_playerComp->GetPlayerInfo()._hp;
	//int maxhp = _eventmanager->_playerComp->GetPlayerInfo()._Maxhp;
	//int minhp = 0;
	//int minhpbar = 0;
	//float hpsize = (currenthp - minhp) / (maxhp - minhp) * (_bosshpsize - minhpbar);
}
