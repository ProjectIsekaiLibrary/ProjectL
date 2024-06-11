#pragma once
#include "CommonHeader.h"
#include "KunrealAPI.h"

namespace KunrealEngine
{
	//일시정지 메뉴 UI를 불러오는 함수. UI들을 묶은 부모 오브젝트의 포인터를 반환합니다.
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
				// 
				// 타이틀 씬으로 전환되는 코드
				//
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
				//
				//  게임 종료되는 코드
				//
			});

		pauseuibox->SetActive(false);

		return pauseuibox;
	}
}