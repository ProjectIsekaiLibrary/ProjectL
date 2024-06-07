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
		KunrealEngine::GameObject* button_resume;
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

		button_resume = scene.GetCurrentScene()->CreateObject("button_resume");
		button_resume->SetParent(pauseuibox);
		button_resume->AddComponent<ImageRenderer>();
		button_resume->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_resume->GetComponent<ImageRenderer>()->SetPosition(625.0f, 200.0f);
		button_resume->GetComponent<Transform>()->SetScale(2.5f, 1.4f, 1.0f);

		button_title = scene.GetCurrentScene()->CreateObject("button_title");
		button_title->SetParent(pauseuibox);
		button_title->AddComponent<ImageRenderer>();
		button_title->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_title->GetComponent<ImageRenderer>()->SetPosition(625.0f, 430.0f);
		button_title->GetComponent<Transform>()->SetScale(2.5f, 1.4f, 1.0f);

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

		pauseuibox->SetActive(false);

		return pauseuibox;
	}

	static GameObject* MakeOptionUIPack()
	{
		KunrealEngine::SceneManager& scene = KunrealEngine::SceneManager::GetInstance();
		KunrealEngine::GameObject* optionuibox;
		KunrealEngine::GameObject* imagebackground;

		KunrealEngine::GameObject* bgm_text;
		KunrealEngine::GameObject* button_bgm1;
		KunrealEngine::GameObject* button_bgm2;
		KunrealEngine::GameObject* button_bgm3;
		KunrealEngine::GameObject* button_bgm4;
		KunrealEngine::GameObject* button_bgm5;
		KunrealEngine::GameObject* button_bgm6;

		KunrealEngine::GameObject* sfx_text;
		KunrealEngine::GameObject* button_sfx1;
		KunrealEngine::GameObject* button_sfx2;
		KunrealEngine::GameObject* button_sfx3;
		KunrealEngine::GameObject* button_sfx4;
		KunrealEngine::GameObject* button_sfx5;
		KunrealEngine::GameObject* button_sfx6;

		KunrealEngine::GameObject* button_check1;
		KunrealEngine::GameObject* button_check2;
		KunrealEngine::GameObject* fullscreen_text;
		KunrealEngine::GameObject* windowed_text;

		optionuibox = scene.GetCurrentScene()->CreateObject("menuuibox");
		optionuibox->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 0.0f);

		imagebackground = scene.GetCurrentScene()->CreateObject("imagebackground");
		imagebackground->AddComponent<ImageRenderer>();
		imagebackground->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		imagebackground->GetComponent<ImageRenderer>()->SetPosition(480.f, 0.f);
		imagebackground->GetComponent<Transform>()->SetScale(5.0f, 10.0f, 1.0f);
		imagebackground->SetParent(optionuibox);

		/////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////
		//BGM 사운드 조절

		button_bgm1 = scene.GetCurrentScene()->CreateObject("button_bgm1");
		button_bgm1->SetParent(optionuibox);
		button_bgm1->AddComponent<ImageRenderer>();
		button_bgm1->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_bgm1->GetComponent<ImageRenderer>()->SetPosition(790.0f, 339.0f);
		button_bgm1->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 1.0f);

		button_bgm2 = scene.GetCurrentScene()->CreateObject("button_bgm2");
		button_bgm2->SetParent(optionuibox);
		button_bgm2->AddComponent<ImageRenderer>();
		button_bgm2->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_bgm2->GetComponent<ImageRenderer>()->SetPosition(808.0f, 319.0f);
		button_bgm2->GetComponent<Transform>()->SetScale(0.1f, 0.3f, 1.0f);

		button_bgm3 = scene.GetCurrentScene()->CreateObject("button_bgm3");
		button_bgm3->SetParent(optionuibox);
		button_bgm3->AddComponent<ImageRenderer>();
		button_bgm3->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_bgm3->GetComponent<ImageRenderer>()->SetPosition(826.0f, 297.0f);
		button_bgm3->GetComponent<Transform>()->SetScale(0.1f, 0.5f, 1.0f);

		button_bgm4 = scene.GetCurrentScene()->CreateObject("button_bgm4");
		button_bgm4->SetParent(optionuibox);
		button_bgm4->AddComponent<ImageRenderer>();
		button_bgm4->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_bgm4->GetComponent<ImageRenderer>()->SetPosition(844.0f, 276.0f);
		button_bgm4->GetComponent<Transform>()->SetScale(0.1f, 0.7f, 1.0f);

		button_bgm5 = scene.GetCurrentScene()->CreateObject("button_bgm5");
		button_bgm5->SetParent(optionuibox);
		button_bgm5->AddComponent<ImageRenderer>();
		button_bgm5->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_bgm5->GetComponent<ImageRenderer>()->SetPosition(862.0f, 254.0f);
		button_bgm5->GetComponent<Transform>()->SetScale(0.1f, 0.9f, 1.0f);

		button_bgm6 = scene.GetCurrentScene()->CreateObject("button_bgm6");
		button_bgm6->SetParent(optionuibox);
		button_bgm6->AddComponent<ImageRenderer>();
		button_bgm6->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_bgm6->GetComponent<ImageRenderer>()->SetPosition(880.0f, 222.0f);
		button_bgm6->GetComponent<Transform>()->SetScale(0.1f, 1.2f, 1.0f); 

		bgm_text = scene.GetCurrentScene()->CreateObject("button_bgmtext");
		bgm_text->SetParent(optionuibox);
		bgm_text->AddComponent<ImageRenderer>();
		bgm_text->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		bgm_text->GetComponent<ImageRenderer>()->SetPosition(560.0f, 200.0f);
		bgm_text->GetComponent<Transform>()->SetScale(1.0f, 1.4f, 1.0f);

		/////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////
		//SFX 사운드 조절

		button_sfx1 = scene.GetCurrentScene()->CreateObject("button_sfx1");
		button_sfx1->SetParent(optionuibox);
		button_sfx1->AddComponent<ImageRenderer>();
		button_sfx1->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_sfx1->GetComponent<ImageRenderer>()->SetPosition(1200.0f, 339.0f);
		button_sfx1->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 1.0f);
			   
		button_sfx2 = scene.GetCurrentScene()->CreateObject("button_sfx2");
		button_sfx2->SetParent(optionuibox);
		button_sfx2->AddComponent<ImageRenderer>();
		button_sfx2->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_sfx2->GetComponent<ImageRenderer>()->SetPosition(1218.0f, 319.0f);
		button_sfx2->GetComponent<Transform>()->SetScale(0.1f, 0.3f, 1.0f);
			   
		button_sfx3 = scene.GetCurrentScene()->CreateObject("button_sfx3");
		button_sfx3->SetParent(optionuibox);
		button_sfx3->AddComponent<ImageRenderer>();
		button_sfx3->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_sfx3->GetComponent<ImageRenderer>()->SetPosition(1236.0f, 297.0f);
		button_sfx3->GetComponent<Transform>()->SetScale(0.1f, 0.5f, 1.0f);
			   
		button_sfx4 = scene.GetCurrentScene()->CreateObject("button_sfx4");
		button_sfx4->SetParent(optionuibox);
		button_sfx4->AddComponent<ImageRenderer>();
		button_sfx4->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_sfx4->GetComponent<ImageRenderer>()->SetPosition(1254.0f, 276.0f);
		button_sfx4->GetComponent<Transform>()->SetScale(0.1f, 0.7f, 1.0f);
			   
		button_sfx5 = scene.GetCurrentScene()->CreateObject("button_sfx5");
		button_sfx5->SetParent(optionuibox);
		button_sfx5->AddComponent<ImageRenderer>();
		button_sfx5->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_sfx5->GetComponent<ImageRenderer>()->SetPosition(1272.0f, 254.0f);
		button_sfx5->GetComponent<Transform>()->SetScale(0.1f, 0.9f, 1.0f);
			   
		button_sfx6 = scene.GetCurrentScene()->CreateObject("button_sfx6");
		button_sfx6->SetParent(optionuibox);
		button_sfx6->AddComponent<ImageRenderer>();
		button_sfx6->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_sfx6->GetComponent<ImageRenderer>()->SetPosition(1290.0f, 222.0f);
		button_sfx6->GetComponent<Transform>()->SetScale(0.1f, 1.2f, 1.0f);
			   
		sfx_text = scene.GetCurrentScene()->CreateObject("button_sfxtext");
		sfx_text->SetParent(optionuibox);
		sfx_text->AddComponent<ImageRenderer>();
		sfx_text->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		sfx_text->GetComponent<ImageRenderer>()->SetPosition(970.0f, 200.0f);
		sfx_text->GetComponent<Transform>()->SetScale(1.0f, 1.4f, 1.0f);

		/////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////

		button_check1 = scene.GetCurrentScene()->CreateObject("button_check1");
		button_check1->SetParent(optionuibox);
		button_check1->AddComponent<ImageRenderer>();
		button_check1->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_check1->GetComponent<ImageRenderer>()->SetPosition(820.0f, 680.0f);
		button_check1->GetComponent<Transform>()->SetScale(0.3f, 0.1f, 1.0f);

		button_check2 = scene.GetCurrentScene()->CreateObject("button_check2");
		button_check2->SetParent(optionuibox);
		button_check2->AddComponent<ImageRenderer>();
		button_check2->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_check2->GetComponent<ImageRenderer>()->SetPosition(1240.0f, 680.0f);
		button_check2->GetComponent<Transform>()->SetScale(0.3f, 0.1f, 1.0f);

		fullscreen_text = scene.GetCurrentScene()->CreateObject("fullscreen_text");
		fullscreen_text->SetParent(optionuibox);
		fullscreen_text->AddComponent<ImageRenderer>();
		fullscreen_text->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		fullscreen_text->GetComponent<ImageRenderer>()->SetPosition(560.0f, 640.0f);
		fullscreen_text->GetComponent<Transform>()->SetScale(1.0f, 1.4f, 1.0f);

		windowed_text = scene.GetCurrentScene()->CreateObject("windowed_text");
		windowed_text->SetParent(optionuibox);
		windowed_text->AddComponent<ImageRenderer>();
		windowed_text->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		windowed_text->GetComponent<ImageRenderer>()->SetPosition(970.0f, 640.0f);
		windowed_text->GetComponent<Transform>()->SetScale(1.0f, 1.4f, 1.0f);

		optionuibox->SetActive(false);

		return optionuibox;
	}
}