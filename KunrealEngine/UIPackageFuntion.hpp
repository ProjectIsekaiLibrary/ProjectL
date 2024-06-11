#pragma once
#include "CommonHeader.h"
#include "KunrealAPI.h"

namespace KunrealEngine
{
	//전투 UI를 불러오는 함수. UI들을 묶은 부모 오브젝트의 포인터를 반환합니다.
	static GameObject* MakeBattleUIPack()
	{
		KunrealEngine::SceneManager& scene = KunrealEngine::SceneManager::GetInstance();
		KunrealEngine::GameObject* battleuibox;
		KunrealEngine::GameObject* background;
		KunrealEngine::GameObject* ui_skill1;
		KunrealEngine::GameObject* ui_skill2;
		KunrealEngine::GameObject* ui_skill3;
		KunrealEngine::GameObject* ui_skill4;
		KunrealEngine::GameObject* potion;
		KunrealEngine::GameObject* dash;
		KunrealEngine::GameObject* playerhp_bar;
		KunrealEngine::GameObject* bosshp_bar;

		battleuibox = scene.GetCurrentScene()->CreateObject("battleuibox");
		

		background = scene.GetCurrentScene()->CreateObject("background");
		background->SetParent(battleuibox);
		background->AddComponent<ImageRenderer>();
		background->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		background->GetComponent<ImageRenderer>()->SetPosition(460.0f, 815.0f);
		background->GetComponent<Transform>()->SetScale(5.4f, 2.5f, 1.0f);

		ui_skill1 = scene.GetCurrentScene()->CreateObject("ui_skill1");
		ui_skill1->SetParent(battleuibox);
		ui_skill1->AddComponent<ImageRenderer>();
		ui_skill1->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		ui_skill1->GetComponent<ImageRenderer>()->SetPosition(570.f, 910.f);
		ui_skill1->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

		ui_skill2 = scene.GetCurrentScene()->CreateObject("ui_skill2");
		ui_skill2->SetParent(battleuibox);
		ui_skill2->AddComponent<ImageRenderer>();
		ui_skill2->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		ui_skill2->GetComponent<ImageRenderer>()->SetPosition(724.f, 910.f);
		ui_skill2->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

		ui_skill3 = scene.GetCurrentScene()->CreateObject("ui_skill3");
		ui_skill3->SetParent(battleuibox);
		ui_skill3->AddComponent<ImageRenderer>();
		ui_skill3->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		ui_skill3->GetComponent<ImageRenderer>()->SetPosition(880.f, 910.f);
		ui_skill3->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

		ui_skill4 = scene.GetCurrentScene()->CreateObject("ui_skill4");
		ui_skill4->SetParent(battleuibox);
		ui_skill4->AddComponent<ImageRenderer>();
		ui_skill4->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		ui_skill4->GetComponent<ImageRenderer>()->SetPosition(1035.f, 910.f);
		ui_skill4->GetComponent<Transform>()->SetScale(0.7f, 1.21f, 1.0f);

		potion = scene.GetCurrentScene()->CreateObject("ui_skill5");
		potion->SetParent(battleuibox);
		potion->AddComponent<ImageRenderer>();
		potion->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		potion->GetComponent<ImageRenderer>()->SetPosition(1310.f, 940.f);
		potion->GetComponent<Transform>()->SetScale(0.5f, 0.9f, 1.0f);

		dash = scene.GetCurrentScene()->CreateObject("ui_skill6");
		dash->SetParent(battleuibox);
		dash->AddComponent<ImageRenderer>();
		dash->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		dash->GetComponent<ImageRenderer>()->SetPosition(1200.f, 940.f);
		dash->GetComponent<Transform>()->SetScale(0.5f, 0.9f, 1.0f);

		playerhp_bar = scene.GetCurrentScene()->CreateObject("playerhp_bar");
		playerhp_bar->SetParent(battleuibox);
		playerhp_bar->AddComponent<ImageRenderer>();
		playerhp_bar->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		playerhp_bar->GetComponent<ImageRenderer>()->SetPosition(500.f, 850.f);
		playerhp_bar->GetComponent<Transform>()->SetScale(5.0f, 0.3f, 1.0f);

		bosshp_bar = scene.GetCurrentScene()->CreateObject("bosshp_bar");
		bosshp_bar->SetParent(battleuibox);
		bosshp_bar->AddComponent<ImageRenderer>();
		bosshp_bar->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		bosshp_bar->GetComponent<ImageRenderer>()->SetPosition(500.f, 15.f);
		bosshp_bar->GetComponent<Transform>()->SetScale(5.0f, 0.3f, 1.0f);

		battleuibox->SetActive(false);

		return battleuibox;
	}

	//전투 UI를 불러오는 함수. UI들을 묶은 부모 오브젝트의 포인터를 반환합니다.
	static GameObject* MakeMenuUIPack()
	{
		KunrealEngine::SceneManager& scene = KunrealEngine::SceneManager::GetInstance();
		KunrealEngine::GameObject* pauseuibox;
		KunrealEngine::GameObject* button_option;
		KunrealEngine::GameObject* button_title;
		KunrealEngine::GameObject* button_quit;
		KunrealEngine::GameObject* button_exit;
		KunrealEngine::GameObject* imagebackground;

		pauseuibox = scene.GetCurrentScene()->CreateObject("pauseuibox");
		pauseuibox->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 0.0f);

		imagebackground = scene.GetCurrentScene()->CreateObject("imagebackground");
		imagebackground->AddComponent<ImageRenderer>();
		imagebackground->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		imagebackground->GetComponent<ImageRenderer>()->SetPosition(575.f, 100.f);
		imagebackground->GetComponent<Transform>()->SetScale(3.0f, 7.0f, 1.0f);
		imagebackground->SetParent(pauseuibox);

		button_option = scene.GetCurrentScene()->CreateObject("button_resume");
		button_option->SetParent(pauseuibox);
		button_option->AddComponent<ImageRenderer>();
		//button_resume->GetComponent<ImageRenderer>().
		button_option->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_option->GetComponent<ImageRenderer>()->SetPosition(625.0f, 200.0f);
		button_option->GetComponent<Transform>()->SetScale(2.5f, 1.4f, 1.0f);
		button_option->AddComponent<ButtonSystem>();
		button_option->GetComponent<ButtonSystem>()->SetImage(button_option->GetComponent<ImageRenderer>());
		button_option->GetComponent<ButtonSystem>()->SetButtonFunc([pauseuibox]()
			{
				KunrealEngine::SceneManager::GetInstance().GetCurrentScene()->GetGameObject("optionuibox")->SetActive(true);
				pauseuibox->SetActive(false);
			});

		button_title = scene.GetCurrentScene()->CreateObject("button_title");
		button_title->SetParent(pauseuibox);
		button_title->AddComponent<ImageRenderer>();
		button_title->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_title->GetComponent<ImageRenderer>()->SetPosition(625.0f, 430.0f);
		button_title->GetComponent<Transform>()->SetScale(2.5f, 1.4f, 1.0f);
		button_title->AddComponent<ButtonSystem>();
		button_title->GetComponent<ButtonSystem>()->SetImage(button_title->GetComponent<ImageRenderer>());
		button_title->GetComponent<ButtonSystem>()->SetButtonFunc([pauseuibox]()
			{
				pauseuibox->SetActive(false);
			});

		button_quit = scene.GetCurrentScene()->CreateObject("button_Quit");
		button_quit->SetParent(pauseuibox);
		button_quit->AddComponent<ImageRenderer>();
		button_quit->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_quit->GetComponent<ImageRenderer>()->SetPosition(625.0f, 650.0f);
		button_quit->GetComponent<Transform>()->SetScale(2.5f, 1.4f, 1.0f);

		button_exit = scene.GetCurrentScene()->CreateObject("button_exit");
		button_exit->SetParent(pauseuibox);
		button_exit->AddComponent<ImageRenderer>();
		button_exit->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_exit->GetComponent<ImageRenderer>()->SetPosition(1100.f, 75.f);
		button_exit->GetComponent<Transform>()->SetScale(0.47f, 0.78f, 1.0f);
		button_exit->AddComponent<ButtonSystem>();
		button_exit->GetComponent<ButtonSystem>()->SetImage(button_exit->GetComponent<ImageRenderer>());
		button_exit->GetComponent<ButtonSystem>()->SetButtonFunc([pauseuibox]()
			{
				pauseuibox->SetActive(false);
			});

		pauseuibox->SetActive(false);

		return pauseuibox;
	}
}