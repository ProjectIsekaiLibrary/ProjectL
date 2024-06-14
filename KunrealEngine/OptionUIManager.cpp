#include "OptionUIManager.h"
#include "ButtonSystem.h"
#include "SoundSystem.h"

KunrealEngine::OptionUIManager::OptionUIManager()
{
}

KunrealEngine::OptionUIManager::~OptionUIManager()
{
}

void KunrealEngine::OptionUIManager::Initialize()
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
	KunrealEngine::GameObject* button_bgm7;
	KunrealEngine::GameObject* button_bgm8;
	KunrealEngine::GameObject* button_bgm9;
	KunrealEngine::GameObject* button_bgm10;
	KunrealEngine::GameObject* button_bgm11;

	KunrealEngine::GameObject* sfx_text;
	KunrealEngine::GameObject* button_sfx1;
	KunrealEngine::GameObject* button_sfx2;
	KunrealEngine::GameObject* button_sfx3;
	KunrealEngine::GameObject* button_sfx4;
	KunrealEngine::GameObject* button_sfx5;
	KunrealEngine::GameObject* button_sfx6;
	KunrealEngine::GameObject* button_sfx7;
	KunrealEngine::GameObject* button_sfx8;
	KunrealEngine::GameObject* button_sfx9;
	KunrealEngine::GameObject* button_sfx10;
	KunrealEngine::GameObject* button_sfx11;

	KunrealEngine::GameObject* button_check1;
	KunrealEngine::GameObject* button_check2;
	KunrealEngine::GameObject* fullscreen_text;
	KunrealEngine::GameObject* windowed_text;

	KunrealEngine::GameObject* button_exit;

	optionuibox = this->GetOwner();
	optionuibox->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 0.0f);
	optionuibox->AddComponent<SoundPlayer>();

	imagebackground = scene.GetCurrentScene()->CreateObject("imagebackground");
	imagebackground->AddComponent<ImageRenderer>();
	imagebackground->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	imagebackground->GetComponent<ImageRenderer>()->SetPosition(185.f, 110.f);
	imagebackground->GetComponent<Transform>()->SetScale(8.0f, 8.0f, 1.0f);
	imagebackground->SetParent(optionuibox);

	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	//BGM 사운드 조절

	button_bgm1 = scene.GetCurrentScene()->CreateObject("button_bgm1");
	button_bgm1->SetParent(optionuibox);
	button_bgm1->AddComponent<ImageRenderer>();
	button_bgm1->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	button_bgm1->GetComponent<ImageRenderer>()->SetPosition(650.0f, 339.0f);
	button_bgm1->GetComponent<Transform>()->SetScale(0.4f, 0.1f, 1.0f);
	button_bgm1->AddComponent<ButtonSystem>();
	button_bgm1->GetComponent<ButtonSystem>()->SetImage(button_bgm1->GetComponent<ImageRenderer>());
	button_bgm1->GetComponent<ButtonSystem>()->SetButtonFunc([this]() {Setvolumebutton(0, &_bgm_step); });
	_bgms.push_back(button_bgm1);

	button_bgm2 = scene.GetCurrentScene()->CreateObject("button_bgm2");
	button_bgm2->SetParent(optionuibox);
	button_bgm2->AddComponent<ImageRenderer>();
	button_bgm2->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	button_bgm2->GetComponent<ImageRenderer>()->SetPosition(725.0f, 330.0f);
	button_bgm2->GetComponent<Transform>()->SetScale(0.4f, 0.2f, 1.0f);
	button_bgm2->AddComponent<ButtonSystem>();
	button_bgm2->GetComponent<ButtonSystem>()->SetImage(button_bgm2->GetComponent<ImageRenderer>());
	button_bgm2->GetComponent<ButtonSystem>()->SetButtonFunc([this]() {Setvolumebutton(1, &_bgm_step); });
	_bgms.push_back(button_bgm2);

	button_bgm3 = scene.GetCurrentScene()->CreateObject("button_bgm3");
	button_bgm3->SetParent(optionuibox);
	button_bgm3->AddComponent<ImageRenderer>();
	button_bgm3->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	button_bgm3->GetComponent<ImageRenderer>()->SetPosition(800.0f, 319.0f);
	button_bgm3->GetComponent<Transform>()->SetScale(0.4f, 0.3f, 1.0f);
	button_bgm3->AddComponent<ButtonSystem>();
	button_bgm3->GetComponent<ButtonSystem>()->SetImage(button_bgm3->GetComponent<ImageRenderer>());
	button_bgm3->GetComponent<ButtonSystem>()->SetButtonFunc([this]() {Setvolumebutton(2, &_bgm_step); });
	_bgms.push_back(button_bgm3);

	button_bgm4 = scene.GetCurrentScene()->CreateObject("button_bgm4");
	button_bgm4->SetParent(optionuibox);
	button_bgm4->AddComponent<ImageRenderer>();
	button_bgm4->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	button_bgm4->GetComponent<ImageRenderer>()->SetPosition(875.0f, 308.0f);
	button_bgm4->GetComponent<Transform>()->SetScale(0.4f, 0.4f, 1.0f);
	button_bgm4->AddComponent<ButtonSystem>();
	button_bgm4->GetComponent<ButtonSystem>()->SetImage(button_bgm4->GetComponent<ImageRenderer>());
	button_bgm4->GetComponent<ButtonSystem>()->SetButtonFunc([this]() {Setvolumebutton(3, &_bgm_step); });
	_bgms.push_back(button_bgm4);

	button_bgm5 = scene.GetCurrentScene()->CreateObject("button_bgm5");
	button_bgm5->SetParent(optionuibox);
	button_bgm5->AddComponent<ImageRenderer>();
	button_bgm5->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	button_bgm5->GetComponent<ImageRenderer>()->SetPosition(950.0f, 297.0f);
	button_bgm5->GetComponent<Transform>()->SetScale(0.4f, 0.5f, 1.0f);
	button_bgm5->AddComponent<ButtonSystem>();
	button_bgm5->GetComponent<ButtonSystem>()->SetImage(button_bgm5->GetComponent<ImageRenderer>());
	button_bgm5->GetComponent<ButtonSystem>()->SetButtonFunc([this]() {Setvolumebutton(4, &_bgm_step); });
	_bgms.push_back(button_bgm5);

	button_bgm6 = scene.GetCurrentScene()->CreateObject("button_bgm6");
	button_bgm6->SetParent(optionuibox);
	button_bgm6->AddComponent<ImageRenderer>();
	button_bgm6->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	button_bgm6->GetComponent<ImageRenderer>()->SetPosition(1025.0f, 287.0f);
	button_bgm6->GetComponent<Transform>()->SetScale(0.4f, 0.6f, 1.0f);
	button_bgm6->AddComponent<ButtonSystem>();
	button_bgm6->GetComponent<ButtonSystem>()->SetImage(button_bgm6->GetComponent<ImageRenderer>());
	button_bgm6->GetComponent<ButtonSystem>()->SetButtonFunc([this]() {Setvolumebutton(5, &_bgm_step); });
	_bgms.push_back(button_bgm6);

	button_bgm7 = scene.GetCurrentScene()->CreateObject("button_bgm1");
	button_bgm7->SetParent(optionuibox);
	button_bgm7->AddComponent<ImageRenderer>();
	button_bgm7->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	button_bgm7->GetComponent<ImageRenderer>()->SetPosition(1100.0f, 276.0f);
	button_bgm7->GetComponent<Transform>()->SetScale(0.4f, 0.7f, 1.0f);
	button_bgm7->AddComponent<ButtonSystem>();
	button_bgm7->GetComponent<ButtonSystem>()->SetImage(button_bgm7->GetComponent<ImageRenderer>());
	button_bgm7->GetComponent<ButtonSystem>()->SetButtonFunc([this]() {Setvolumebutton(6, &_bgm_step); });
	_bgms.push_back(button_bgm7);

	button_bgm8 = scene.GetCurrentScene()->CreateObject("button_bgm2");
	button_bgm8->SetParent(optionuibox);
	button_bgm8->AddComponent<ImageRenderer>();
	button_bgm8->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	button_bgm8->GetComponent<ImageRenderer>()->SetPosition(1175.0f, 265.0f);
	button_bgm8->GetComponent<Transform>()->SetScale(0.4f, 0.8f, 1.0f);
	button_bgm8->AddComponent<ButtonSystem>();
	button_bgm8->GetComponent<ButtonSystem>()->SetImage(button_bgm8->GetComponent<ImageRenderer>());
	button_bgm8->GetComponent<ButtonSystem>()->SetButtonFunc([this]() {Setvolumebutton(7, &_bgm_step); });
	_bgms.push_back(button_bgm8);

	button_bgm9 = scene.GetCurrentScene()->CreateObject("button_bgm3");
	button_bgm9->SetParent(optionuibox);
	button_bgm9->AddComponent<ImageRenderer>();
	button_bgm9->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	button_bgm9->GetComponent<ImageRenderer>()->SetPosition(1250.0f, 254.0f);
	button_bgm9->GetComponent<Transform>()->SetScale(0.4f, 0.9f, 1.0f);
	button_bgm9->AddComponent<ButtonSystem>();
	button_bgm9->GetComponent<ButtonSystem>()->SetImage(button_bgm9->GetComponent<ImageRenderer>());
	button_bgm9->GetComponent<ButtonSystem>()->SetButtonFunc([this]() {Setvolumebutton(8, &_bgm_step); });
	_bgms.push_back(button_bgm9);

	button_bgm10 = scene.GetCurrentScene()->CreateObject("button_bgm4");
	button_bgm10->SetParent(optionuibox);
	button_bgm10->AddComponent<ImageRenderer>();
	button_bgm10->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	button_bgm10->GetComponent<ImageRenderer>()->SetPosition(1325.0f, 243.0f);
	button_bgm10->GetComponent<Transform>()->SetScale(0.4f, 1.0f, 1.0f);
	button_bgm10->AddComponent<ButtonSystem>();
	button_bgm10->GetComponent<ButtonSystem>()->SetImage(button_bgm10->GetComponent<ImageRenderer>());
	button_bgm10->GetComponent<ButtonSystem>()->SetButtonFunc([this]() {Setvolumebutton(9, &_bgm_step); });
	_bgms.push_back(button_bgm10);

	button_bgm11 = scene.GetCurrentScene()->CreateObject("button_bgm5");
	button_bgm11->SetParent(optionuibox);
	button_bgm11->AddComponent<ImageRenderer>();
	button_bgm11->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	button_bgm11->GetComponent<ImageRenderer>()->SetPosition(1400.0f, 233.0f);
	button_bgm11->GetComponent<Transform>()->SetScale(0.4f, 1.1f, 1.0f);
	button_bgm11->AddComponent<ButtonSystem>();
	button_bgm11->GetComponent<ButtonSystem>()->SetImage(button_bgm11->GetComponent<ImageRenderer>());
	button_bgm11->GetComponent<ButtonSystem>()->SetButtonFunc([this]() {Setvolumebutton(10, &_bgm_step); });
	_bgms.push_back(button_bgm11);

	bgm_text = scene.GetCurrentScene()->CreateObject("button_bgmtext");
	bgm_text->SetParent(optionuibox);
	bgm_text->AddComponent<ImageRenderer>();
	bgm_text->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	bgm_text->GetComponent<ImageRenderer>()->SetPosition(350, 250);
	bgm_text->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);

	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	//SFX 사운드 조절

	button_sfx1 = scene.GetCurrentScene()->CreateObject("button_sfx1");
	button_sfx1->SetParent(optionuibox);
	button_sfx1->AddComponent<ImageRenderer>();
	button_sfx1->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	button_sfx1->GetComponent<ImageRenderer>()->SetPosition(650.0f, 506.0f);
	button_sfx1->GetComponent<Transform>()->SetScale(0.4f, 0.1f, 1.0f);
	button_sfx1->AddComponent<ButtonSystem>();
	button_sfx1->GetComponent<ButtonSystem>()->SetImage(button_sfx1->GetComponent<ImageRenderer>());
	button_sfx1->GetComponent<ButtonSystem>()->SetButtonFunc([this]() {Setvolumebutton(0, &_sfx_step); });
	_sfxs.push_back(button_sfx1);

	button_sfx2 = scene.GetCurrentScene()->CreateObject("button_sfx2");
	button_sfx2->SetParent(optionuibox);
	button_sfx2->AddComponent<ImageRenderer>();
	button_sfx2->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	button_sfx2->GetComponent<ImageRenderer>()->SetPosition(725.0f, 496.0f);
	button_sfx2->GetComponent<Transform>()->SetScale(0.4f, 0.2f, 1.0f);
	button_sfx2->AddComponent<ButtonSystem>();
	button_sfx2->GetComponent<ButtonSystem>()->SetImage(button_sfx2->GetComponent<ImageRenderer>());
	button_sfx2->GetComponent<ButtonSystem>()->SetButtonFunc([this]() {Setvolumebutton(1, &_sfx_step); });
	_sfxs.push_back(button_sfx2);

	button_sfx3 = scene.GetCurrentScene()->CreateObject("button_sfx3");
	button_sfx3->SetParent(optionuibox);
	button_sfx3->AddComponent<ImageRenderer>();
	button_sfx3->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	button_sfx3->GetComponent<ImageRenderer>()->SetPosition(800.0f, 485.0f);
	button_sfx3->GetComponent<Transform>()->SetScale(0.4f, 0.3f, 1.0f);
	button_sfx3->AddComponent<ButtonSystem>();
	button_sfx3->GetComponent<ButtonSystem>()->SetImage(button_sfx3->GetComponent<ImageRenderer>());
	button_sfx3->GetComponent<ButtonSystem>()->SetButtonFunc([this]() {Setvolumebutton(2, &_sfx_step); });
	_sfxs.push_back(button_sfx3);

	button_sfx4 = scene.GetCurrentScene()->CreateObject("button_sfx4");
	button_sfx4->SetParent(optionuibox);
	button_sfx4->AddComponent<ImageRenderer>();
	button_sfx4->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	button_sfx4->GetComponent<ImageRenderer>()->SetPosition(875.0f, 474.0f);
	button_sfx4->GetComponent<Transform>()->SetScale(0.4f, 0.4f, 1.0f);
	button_sfx4->AddComponent<ButtonSystem>();
	button_sfx4->GetComponent<ButtonSystem>()->SetImage(button_sfx4->GetComponent<ImageRenderer>());
	button_sfx4->GetComponent<ButtonSystem>()->SetButtonFunc([this]() {Setvolumebutton(3, &_sfx_step); });
	_sfxs.push_back(button_sfx4);

	button_sfx5 = scene.GetCurrentScene()->CreateObject("button_sfx5");
	button_sfx5->SetParent(optionuibox);
	button_sfx5->AddComponent<ImageRenderer>();
	button_sfx5->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	button_sfx5->GetComponent<ImageRenderer>()->SetPosition(950.0f, 463.0f);
	button_sfx5->GetComponent<Transform>()->SetScale(0.4f, 0.5f, 1.0f);
	button_sfx5->AddComponent<ButtonSystem>();
	button_sfx5->GetComponent<ButtonSystem>()->SetImage(button_sfx5->GetComponent<ImageRenderer>());
	button_sfx5->GetComponent<ButtonSystem>()->SetButtonFunc([this]() {Setvolumebutton(4, &_sfx_step); });
	_sfxs.push_back(button_sfx5);

	button_sfx6 = scene.GetCurrentScene()->CreateObject("button_sfx6");
	button_sfx6->SetParent(optionuibox);
	button_sfx6->AddComponent<ImageRenderer>();
	button_sfx6->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	button_sfx6->GetComponent<ImageRenderer>()->SetPosition(1025.0f, 453.0f);
	button_sfx6->GetComponent<Transform>()->SetScale(0.4f, 0.6f, 1.0f);
	button_sfx6->AddComponent<ButtonSystem>();
	button_sfx6->GetComponent<ButtonSystem>()->SetImage(button_sfx6->GetComponent<ImageRenderer>());
	button_sfx6->GetComponent<ButtonSystem>()->SetButtonFunc([this]() {Setvolumebutton(5, &_sfx_step); });
	_sfxs.push_back(button_sfx6);

	button_sfx7 = scene.GetCurrentScene()->CreateObject("button_sfx1");
	button_sfx7->SetParent(optionuibox);
	button_sfx7->AddComponent<ImageRenderer>();
	button_sfx7->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	button_sfx7->GetComponent<ImageRenderer>()->SetPosition(1100.0f, 442.0f);
	button_sfx7->GetComponent<Transform>()->SetScale(0.4f, 0.7f, 1.0f);
	button_sfx7->AddComponent<ButtonSystem>();
	button_sfx7->GetComponent<ButtonSystem>()->SetImage(button_sfx7->GetComponent<ImageRenderer>());
	button_sfx7->GetComponent<ButtonSystem>()->SetButtonFunc([this]() {Setvolumebutton(6, &_sfx_step); });
	_sfxs.push_back(button_sfx7);

	button_sfx8 = scene.GetCurrentScene()->CreateObject("button_sfx2");
	button_sfx8->SetParent(optionuibox);
	button_sfx8->AddComponent<ImageRenderer>();
	button_sfx8->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	button_sfx8->GetComponent<ImageRenderer>()->SetPosition(1175.0f, 431.0f);
	button_sfx8->GetComponent<Transform>()->SetScale(0.4f, 0.8f, 1.0f);
	button_sfx8->AddComponent<ButtonSystem>();
	button_sfx8->GetComponent<ButtonSystem>()->SetImage(button_sfx8->GetComponent<ImageRenderer>());
	button_sfx8->GetComponent<ButtonSystem>()->SetButtonFunc([this]() {Setvolumebutton(7, &_sfx_step); });
	_sfxs.push_back(button_sfx8);

	button_sfx9 = scene.GetCurrentScene()->CreateObject("button_sfx3");
	button_sfx9->SetParent(optionuibox);
	button_sfx9->AddComponent<ImageRenderer>();
	button_sfx9->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	button_sfx9->GetComponent<ImageRenderer>()->SetPosition(1250.0f, 420.0f);
	button_sfx9->GetComponent<Transform>()->SetScale(0.4f, 0.9f, 1.0f);
	button_sfx9->AddComponent<ButtonSystem>();
	button_sfx9->GetComponent<ButtonSystem>()->SetImage(button_sfx9->GetComponent<ImageRenderer>());
	button_sfx9->GetComponent<ButtonSystem>()->SetButtonFunc([this]() {Setvolumebutton(8, &_sfx_step); });
	_sfxs.push_back(button_sfx9);

	button_sfx10 = scene.GetCurrentScene()->CreateObject("button_sfx4");
	button_sfx10->SetParent(optionuibox);
	button_sfx10->AddComponent<ImageRenderer>();
	button_sfx10->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	button_sfx10->GetComponent<ImageRenderer>()->SetPosition(1325.0f, 409.0f);
	button_sfx10->GetComponent<Transform>()->SetScale(0.4f, 1.0f, 1.0f);
	button_sfx10->AddComponent<ButtonSystem>();
	button_sfx10->GetComponent<ButtonSystem>()->SetImage(button_sfx10->GetComponent<ImageRenderer>());
	button_sfx10->GetComponent<ButtonSystem>()->SetButtonFunc([this]() {Setvolumebutton(9, &_sfx_step); });
	_sfxs.push_back(button_sfx10);

	button_sfx11 = scene.GetCurrentScene()->CreateObject("button_sfx5");
	button_sfx11->SetParent(optionuibox);
	button_sfx11->AddComponent<ImageRenderer>();
	button_sfx11->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
	button_sfx11->GetComponent<ImageRenderer>()->SetPosition(1400.0f, 399.0f);
	button_sfx11->GetComponent<Transform>()->SetScale(0.4f, 1.1f, 1.0f);
	button_sfx11->AddComponent<ButtonSystem>();
	button_sfx11->GetComponent<ButtonSystem>()->SetImage(button_sfx11->GetComponent<ImageRenderer>());
	button_sfx11->GetComponent<ButtonSystem>()->SetButtonFunc([this]() {Setvolumebutton(10, &_sfx_step); });
	_sfxs.push_back(button_sfx11);

	sfx_text = scene.GetCurrentScene()->CreateObject("button_sfxtext");
	sfx_text->SetParent(optionuibox);
	sfx_text->AddComponent<ImageRenderer>();
	sfx_text->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	sfx_text->GetComponent<ImageRenderer>()->SetPosition(350.0f, 400.0f);
	sfx_text->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);

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
	fullscreen_text->GetComponent<ImageRenderer>()->SetPosition(400.0f, 640.0f);
	fullscreen_text->GetComponent<Transform>()->SetScale(1.2f, 1.2f, 1.0f);

	windowed_text = scene.GetCurrentScene()->CreateObject("windowed_text");
	windowed_text->SetParent(optionuibox);
	windowed_text->AddComponent<ImageRenderer>();
	windowed_text->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	windowed_text->GetComponent<ImageRenderer>()->SetPosition(1100.0f, 640.0f);
	windowed_text->GetComponent<Transform>()->SetScale(1.2f, 1.2f, 1.0f);

	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////

	button_exit = scene.GetCurrentScene()->CreateObject("button_exit");
	button_exit->SetParent(optionuibox);
	button_exit->AddComponent<ImageRenderer>();
	button_exit->GetComponent<ImageRenderer>()->SetImage("backposition.png");
	button_exit->GetComponent<ImageRenderer>()->SetPosition(1600.f, 130.f);
	button_exit->GetComponent<Transform>()->SetScale(0.47f, 0.78f, 1.0f);
	button_exit->AddComponent<ButtonSystem>();
	button_exit->GetComponent<ButtonSystem>()->SetImage(button_exit->GetComponent<ImageRenderer>());
	button_exit->GetComponent<ButtonSystem>()->SetButtonFunc([optionuibox]()
		{
			KunrealEngine::SceneManager::GetInstance().GetCurrentScene()->GetGameObject("pauseuibox")->SetActive(true);
			optionuibox->SetActive(false);
		});

	optionuibox->SetActive(false);
}

void KunrealEngine::OptionUIManager::Release()
{
}

void KunrealEngine::OptionUIManager::FixedUpdate()
{
}

void KunrealEngine::OptionUIManager::Update()
{
	static int prevbgm = -1;
	static int prevsfx = -1;

	if (prevbgm != _bgm_step)
	{
		for (int a = 0 ; a < _bgm_step + 1; a++)
		{
			_bgms[a]->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
		}

		for (int b = _bgm_step + 1 ; b < 11 ; b++)
		{
			_bgms[b]->GetComponent<ImageRenderer>()->SetImage("volume-empty.png");
		}
	}

	if (prevsfx != _sfx_step)
	{
		for (int a = 0; a < _sfx_step + 1; a++)
		{
			_sfxs[a]->GetComponent<ImageRenderer>()->SetImage("volume-in.png");
		}

		for (int b = _sfx_step + 1; b < 11; b++)
		{
			_sfxs[b]->GetComponent<ImageRenderer>()->SetImage("volume-empty.png");
		}
	}


	prevbgm = _bgm_step;
	prevsfx = _sfx_step;
}

void KunrealEngine::OptionUIManager::LateUpdate()
{
}

void KunrealEngine::OptionUIManager::OnTriggerEnter()
{
}

void KunrealEngine::OptionUIManager::OnTriggerStay()
{
}

void KunrealEngine::OptionUIManager::OnTriggerExit()
{
}

void KunrealEngine::OptionUIManager::SetActive(bool active)
{
}

///////////////////////////////////
///////////////////////////////////
///////////////////////////////////

void KunrealEngine::OptionUIManager::Setvolumebutton(int vol, int* type)
{
	*type = vol;

	if (type == &_bgm_step)
	{
		SoundSystem::GetInstance().SetvolumeGroup(SOUNDTYPE::BGM, vol);
	} 
	else if(type == &_sfx_step)
	{
		SoundSystem::GetInstance().SetvolumeGroup(SOUNDTYPE::SFX, vol);
	}
}
