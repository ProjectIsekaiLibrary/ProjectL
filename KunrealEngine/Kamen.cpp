#include <functional>
#include "Kamen.h"
//#include "Coroutine.h"

//Coroutine_Func coro()
//{
//	Waitforsecond(3.0);
//	_status = BossStatus::IDLE;
//}

KunrealEngine::Kamen::Kamen()
	: Boss()
{
	BossBasicInfo info;

	info.SetHp(10000).SetPhase(3).SetArmor(10).SetDamage(100).SetHp(10000).SetMoveSpeed(1.0f).SetsStaggeredGauge(100.0f);

	SetInfo(info);

	std::function<void()> logic = [this]() 
	{
		TestPattern1();
		//Startcoroutine(coro);
	};

	BossPattern* pattern1 = new BossPattern();

	

	pattern1->SetAnimName("").SetDamage(100.0f).SetSpeed(100.0f).SetRange(20.0f).SetAfterDelay(5.0f);
	pattern1->SetIsWarning(false).SetWarningName("").SetLogic(logic).SetTriggerHp(20.0f);

	BossPattern* pattern2 = new BossPattern();
	pattern2->SetTriggerHp(50.0f).SetRange(30.0f);

	_testPattern.emplace_back(pattern1);

	SetBasicPatternList(&_testPattern);

	SetCorePatternList(&_testPattern);
}

KunrealEngine::Kamen::~Kamen()
{

}

void KunrealEngine::Kamen::Initialize()
{
	Boss::Initialize(this->GetOwner());
}

void KunrealEngine::Kamen::Release()
{

}

void KunrealEngine::Kamen::FixedUpdate()
{

}

void KunrealEngine::Kamen::Update()
{
	Boss::Update();
}

void KunrealEngine::Kamen::LateUpdate()
{

}

void KunrealEngine::Kamen::OnTriggerEnter()
{

}

void KunrealEngine::Kamen::OnTriggerStay()
{

}

void KunrealEngine::Kamen::OnTriggerExit()
{

}

void KunrealEngine::Kamen::SetActive(bool active)
{

}

void KunrealEngine::Kamen::TestPattern1()
{
	auto isAnimationPlaying = _boss->GetComponent<Animator>()->Play("Slash", 20.0f, false);

	if (isAnimationPlaying == false)
	{
		_boss->GetComponent<Animator>()->Stop();

		_status = BossStatus::PATTERN_END;
	}
}
