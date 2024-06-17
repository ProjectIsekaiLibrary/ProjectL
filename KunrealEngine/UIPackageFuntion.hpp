#pragma once
#include <windows.h>
#include "CommonHeader.h"
#include "KunrealAPI.h"
#include "Coroutine.h"

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
				KunrealEngine::SceneManager::GetInstance().GetCurrentScene()->GetGameObject("Option")->SetActive(true);
				pauseuibox->SetActive(false);
			});


		//타이틀로 돌아가기
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
				// KunrealEngine::SceneManager::GetInstance().ChangeScene("Title");
				//
			});


		// 게임 종료
		button_quit = scene.GetCurrentScene()->CreateObject("button_Quit");
		button_quit->SetParent(pauseuibox);
		button_quit->AddComponent<ImageRenderer>();
		button_quit->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_quit->GetComponent<ImageRenderer>()->SetPosition(625.0f, 650.0f);
		button_quit->GetComponent<Transform>()->SetScale(2.5f, 1.4f, 1.0f);
		button_quit->AddComponent<ButtonSystem>();
		button_quit->GetComponent<ButtonSystem>()->SetImage(button_quit->GetComponent<ImageRenderer>());
		button_quit->GetComponent<ButtonSystem>()->SetButtonFunc([pauseuibox]()
			{
				pauseuibox->SetActive(false);
				//
				//  게임 종료되는 코드
				//
				//DestroyWindow();
				PostQuitMessage(0);
			});


		// 메뉴 나가기
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

	//일시정지 메뉴 UI를 불러오는 함수. UI들을 묶은 부모 오브젝트의 포인터를 반환합니다.
	static GameObject* MakeTitleUIPack()
	{
		KunrealEngine::SceneManager& scene = KunrealEngine::SceneManager::GetInstance();
		KunrealEngine::GameObject* titleuibox;
		KunrealEngine::GameObject* title_image;
		KunrealEngine::GameObject* button_Start;
		KunrealEngine::GameObject* button_quit;

		titleuibox = scene.GetCurrentScene()->CreateObject("titleuibox");
		titleuibox->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 0.0f);

		title_image = scene.GetCurrentScene()->CreateObject("Title_Image");
		title_image->AddComponent<ImageRenderer>();
		title_image->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		title_image->GetComponent<ImageRenderer>()->SetPosition(525.f, 130.f);
		title_image->GetComponent<Transform>()->SetScale(4.37f, 1.69f, 1.0f);
		title_image->SetParent(titleuibox);

		// 게임 시작
		// 페이드 아웃 된다던가, 위나 아래로 올라가며 사라진다던가 하는 연출은 전달받은게 없으므로 일단은 disable
		button_Start = scene.GetCurrentScene()->CreateObject("button_Start");
		button_Start->SetParent(titleuibox);
		button_Start->AddComponent<ImageRenderer>();
		button_Start->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_Start->GetComponent<ImageRenderer>()->SetPosition(747.0f, 605.0f);
		button_Start->GetComponent<Transform>()->SetScale(2.00f, 1.50f, 1.0f);
		button_Start->AddComponent<ButtonSystem>();
		button_Start->GetComponent<ButtonSystem>()->SetImage(button_Start->GetComponent<ImageRenderer>());
		button_Start->GetComponent<ButtonSystem>()->SetButtonFunc([titleuibox]()
			{
				titleuibox->GetChild("button_Start")->GetComponent<ButtonSystem>()->SetActive(false);
				Coroutine_staticFunc(disable_staritng, titleuibox)
				{
					auto titleobject = titleuibox;
					while (1)
					{
						Transform* trans = titleobject->GetChild("button_Start")->GetComponent<Transform>();
						float		x = trans->GetPosition().x;
						float		y = trans->GetPosition().y;
						float		z = trans->GetPosition().z;
						trans->SetPosition(x, y + 10, z);

						Transform* trans2 = titleobject->GetChild("Title_Image")->GetComponent<Transform>();
						x = trans2->GetPosition().x;
						y = trans2->GetPosition().y;
						z = trans2->GetPosition().z;
						trans2->SetPosition(x, y - 10, z);

						if (trans->GetPosition().y > 1080 && trans2->GetPosition().y < 0)
						{
							titleobject->SetActive(false);
							//KunrealEngine::SceneManager::GetInstance().ChangeScene("MAIN_GAME");
							break;
						}
						Return_null;
					}
				};
				Startcoroutine(disable_staritng);
			});

		// 게임 종료
		button_quit = scene.GetCurrentScene()->CreateObject("button_Quit");
		button_quit->SetParent(titleuibox);
		button_quit->AddComponent<ImageRenderer>();
		button_quit->GetComponent<ImageRenderer>()->SetImage("backposition.png");
		button_quit->GetComponent<ImageRenderer>()->SetPosition(1845.0f, 1005.0f);
		button_quit->GetComponent<Transform>()->SetScale(0.35f, 0.7f, 1.0f);
		button_quit->AddComponent<ButtonSystem>();
		button_quit->GetComponent<ButtonSystem>()->SetImage(button_quit->GetComponent<ImageRenderer>());
		button_quit->GetComponent<ButtonSystem>()->SetButtonFunc([titleuibox]()
			{
				titleuibox->SetActive(false);
				//
				//  게임 종료되는 코드
				//
				//DestroyWindow();
				PostQuitMessage(0);
			});

		titleuibox->SetActive(false);

		return titleuibox;
	}
}