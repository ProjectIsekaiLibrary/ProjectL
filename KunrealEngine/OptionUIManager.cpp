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
	scene = KunrealEngine::SceneManager::GetInstance().GetCurrentScene();
	KunrealEngine::GameObject* imagebackgroundmiddle;
	KunrealEngine::GameObject* imagebackgroundfill;
	KunrealEngine::GameObject* imagebackgroundL;
	KunrealEngine::GameObject* imagebackgroundR;

	KunrealEngine::GameObject* button_bgmon;
	KunrealEngine::GameObject* button_sfxon;

	KunrealEngine::GameObject* button_bgm;
	KunrealEngine::GameObject* button_sfx;

	KunrealEngine::GameObject* button_check1;
	KunrealEngine::GameObject* button_check2;
	KunrealEngine::GameObject* fullscreen_text;
	KunrealEngine::GameObject* windowed_text;

	KunrealEngine::GameObject* button_exit;
	KunrealEngine::GameObject* button_exit_focus;
	KunrealEngine::GameObject* button_exit_text;

	optionuibox = this->GetOwner();
	optionuibox->GetComponent<Transform>()->SetPosition(320.0f, 0.0f, 0.0f);
	optionuibox->AddComponent<SoundPlayer>();

	imagebackgroundfill = scene->CreateObject("background_f");
	imagebackgroundfill->SetParent(optionuibox);
	imagebackgroundfill->AddComponent<ImageRenderer>();
	imagebackgroundfill->GetComponent<ImageRenderer>()->SetImage("ui/freebox-middle.png");
	imagebackgroundfill->GetComponent<ImageRenderer>()->SetPosition(440.f, 75.f);
	imagebackgroundfill->GetComponent<Transform>()->SetScale(5.0f, 3.0f, 1.0f);

	imagebackgroundmiddle = scene->CreateObject("background_m");
	imagebackgroundmiddle->SetParent(optionuibox);
	imagebackgroundmiddle->AddComponent<ImageRenderer>();
	imagebackgroundmiddle->GetComponent<ImageRenderer>()->SetImage("ui/freebox-constmiddle.png");
	imagebackgroundmiddle->GetComponent<ImageRenderer>()->SetPosition(900.f, 75.f);
	imagebackgroundmiddle->GetComponent<Transform>()->SetScale(3.0f, 3.0f, 1.0f);


	imagebackgroundL = scene->CreateObject("background_L");
	imagebackgroundL->SetParent(optionuibox);
	imagebackgroundL->AddComponent<ImageRenderer>();
	imagebackgroundL->GetComponent<ImageRenderer>()->SetImage("ui/freebox-sideL.png");
	imagebackgroundL->GetComponent<ImageRenderer>()->SetPosition(305.f, 75.f);
	imagebackgroundL->GetComponent<Transform>()->SetScale(3.0f, 3.0f, 1.0f);

	imagebackgroundR = scene->CreateObject("background_R");
	imagebackgroundR->SetParent(optionuibox);
	imagebackgroundR->AddComponent<ImageRenderer>();
	imagebackgroundR->GetComponent<ImageRenderer>()->SetImage("ui/freebox-sideR.png");
	imagebackgroundR->GetComponent<ImageRenderer>()->SetPosition(1500.f,  75.f);
	imagebackgroundR->GetComponent<Transform>()->SetScale(3.0f, 3.0f, 1.0f);

	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	//BGM 사운드 조절
	InitializeVolumeButton(_bgms, &_bgm_step, DirectX::XMFLOAT2{ 700.0f, 350.0f });

	button_bgmon = scene->CreateObject("button_bgmon");
	button_bgmon->SetParent(optionuibox);
	button_bgmon->AddComponent<ImageRenderer>();
	button_bgmon->GetComponent<ImageRenderer>()->SetImage("ui/sound-on.png");
	button_bgmon->GetComponent<ImageRenderer>()->SetPosition(670.0f, 290.0f);
	button_bgmon->GetComponent<Transform>()->SetScale(0.7f, 0.7f, 0.7f);
	button_bgmon->AddComponent<ButtonSystem>();
	button_bgmon->GetComponent<ButtonSystem>()->SetImage(button_bgmon->GetComponent<ImageRenderer>());
	button_bgmon->GetComponent<ButtonSystem>()->SetButtonFunc([this]()
		{
			auto image = SceneManager::GetInstance().GetCurrentScene()->GetGameObject("button_bgmon")->GetComponent<ImageRenderer>();
			std::string path = "Resources/Textures/";
			if (image->GetImageName() == path + "ui/sound-on.png")
			{
				image->ChangeImage("ui/sound-off.png");
				_bgm_step = 0;
			}

			else if (image->GetImageName() == path + "ui/sound-off.png")
			{
				image->ChangeImage("ui/sound-on.png");
				_bgm_step = 1;
			}
		});

	button_bgm = scene->CreateObject("button_bgm");
	button_bgm->SetParent(optionuibox);
	button_bgm->AddComponent<ImageRenderer>();
	button_bgm->GetComponent<ImageRenderer>()->SetImage("ui/BGM_text.png");
	button_bgm->GetComponent<ImageRenderer>()->SetPosition(405.0f, 274.0f);
	button_bgm->GetComponent<Transform>()->SetScale(0.7f, 0.7f, 0.7f);

	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	//SFX 사운드 조절
	InitializeVolumeButton(_sfxs, &_sfx_step, DirectX::XMFLOAT2{ 700.0f, 500.0f });

	button_sfxon = scene->CreateObject("button_sfxon");
	button_sfxon->SetParent(optionuibox);
	button_sfxon->AddComponent<ImageRenderer>();
	button_sfxon->GetComponent<ImageRenderer>()->SetImage("ui/sound-on.png");
	button_sfxon->GetComponent<ImageRenderer>()->SetPosition(670.0f, 440.0f);
	button_sfxon->GetComponent<Transform>()->SetScale(0.7, 0.7, 0.7f);
	button_sfxon->AddComponent<ButtonSystem>();
	button_sfxon->GetComponent<ButtonSystem>()->SetImage(button_sfxon->GetComponent<ImageRenderer>());
	button_sfxon->GetComponent<ButtonSystem>()->SetButtonFunc([this]()
		{
			auto image = SceneManager::GetInstance().GetCurrentScene()->GetGameObject("button_sfxon")->GetComponent<ImageRenderer>();
			std::string path = "Resources/Textures/";
			if (image->GetImageName() == path + "ui/sound-on.png")
			{
				image->ChangeImage("ui/sound-off.png");
				_sfx_step = 0;
			}

			else if (image->GetImageName() == path + "ui/sound-off.png")
			{
				image->ChangeImage("ui/sound-on.png");
				_sfx_step = 1;
			}
		});

	button_sfx = scene->CreateObject("button_sfx");
	button_sfx->SetParent(optionuibox);
	button_sfx->AddComponent<ImageRenderer>();
	button_sfx->GetComponent<ImageRenderer>()->SetImage("ui/SFX_text.png");
	button_sfx->GetComponent<ImageRenderer>()->SetPosition(405.0f, 405.0f);
	button_sfx->GetComponent<Transform>()->SetScale(0.7, 0.7, 0.7f);

	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////

	button_exit = scene->CreateObject("button_exit");
	button_exit->SetParent(optionuibox);
	button_exit->AddComponent<ImageRenderer>();
	button_exit->GetComponent<ImageRenderer>()->SetImage("ui/button-short.png");
	button_exit->GetComponent<ImageRenderer>()->SetPosition(1468.f, 114.f);
	button_exit->GetComponent<Transform>()->SetScale(1.2f, 1.2f, 1.0f);
	button_exit->AddComponent<ButtonSystem>();
	button_exit->GetComponent<ButtonSystem>()->SetImage(button_exit->GetComponent<ImageRenderer>());
	button_exit->GetComponent<ButtonSystem>()->SetButtonFunc([this]()
		{
			SceneManager::GetInstance().GetCurrentScene()->GetGameObject("pauseuibox")->SetActive(true);
			this->optionuibox->SetActive(false);
		});

	button_exit_focus = scene->CreateObject("button_exit");
	button_exit_focus->SetParent(optionuibox);
	button_exit_focus->AddComponent<ImageRenderer>();
	button_exit_focus->GetComponent<ImageRenderer>()->SetImage("ui/button-short-focus.png");
	button_exit_focus->GetComponent<ImageRenderer>()->SetPosition(1468.f, 114.f);
	button_exit_focus->GetComponent<Transform>()->SetScale(1.2f, 1.2f, 1.0f);
	button_exit->GetComponent<ButtonSystem>()->Setfocused(button_exit_focus->GetComponent<ImageRenderer>());
	button_exit_focus = scene->CreateObject("button_exit");
	
	button_exit_focus->SetParent(optionuibox);
	button_exit_focus->AddComponent<ImageRenderer>();
	button_exit_focus->GetComponent<ImageRenderer>()->SetImage("ui/UiTexts/Xbutton.png");
	button_exit_focus->GetComponent<ImageRenderer>()->SetPosition(1468.f, 114.f);
	button_exit_focus->GetComponent<Transform>()->SetScale(1.2f, 1.2f, 1.0f);

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
		for (int a = 0 ; a < _bgm_step ; a++)
		{
			_bgms[a]->GetComponent<ImageRenderer>()->ChangeImage("ui/volume-in.png");
		}

		for (int b = _bgm_step ; b < 10 ; b++)
		{
			_bgms[b]->GetComponent<ImageRenderer>()->ChangeImage("ui/volume-empty.png");
		}
	}

	if (prevsfx != _sfx_step)
	{
		for (int a = 0 ; a < _sfx_step; a++)
		{
			_sfxs[a]->GetComponent<ImageRenderer>()->ChangeImage("ui/volume-in.png");
		}

		for (int b = _sfx_step ; b < 10; b++)
		{
			_sfxs[b]->GetComponent<ImageRenderer>()->ChangeImage("ui/volume-empty.png");
		}
	}

	if (_bgm_step > 0 && prevbgm == 0)
	{
		scene->GetGameObject("button_bgmon")->GetComponent<ImageRenderer>()->ChangeImage("ui/sound-on.png");
	}

	if (_sfx_step > 0 && prevsfx == 0)
	{
		scene->GetGameObject("button_sfxon")->GetComponent<ImageRenderer>()->ChangeImage("ui/sound-on.png");
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

void KunrealEngine::OptionUIManager::InitializeVolumeButton(std::vector<GameObject*>& vecs, int* steps, DirectX::XMFLOAT2 tlposition)
{
	float yscale = 10.0f;
	float xscale = 7.0f;

	for (int step = 1 ; step < 11 ; step++)    
	{
		KunrealEngine::GameObject* bgm;

		// 오브젝트 기본 세팅
		bgm = scene->CreateObject("button");
		vecs.push_back(bgm);

		bgm->SetParent(optionuibox);
		bgm->AddComponent<ImageRenderer>();
		bgm->GetComponent<ImageRenderer>()->SetImage("ui/volume-in.png");
		bgm->GetComponent<ImageRenderer>()->SetPosition(tlposition.x, tlposition.y);
		bgm->GetComponent<Transform>()->SetScale(xscale, yscale, 1.0f);
		bgm->AddComponent<ImageRenderer>();
		bgm->AddComponent<ButtonSystem>();
		bgm->GetComponent<ButtonSystem>()->SetImage(bgm->GetComponent<ImageRenderer>());
		bgm->GetComponent<ButtonSystem>()->SetButtonFunc([this, step, steps]()
			{
				Setvolumebutton(step, steps);
			});

		// 실 오브젝트 위치 세팅
		// Y포지션
		float blposition = bgm->GetComponent<ImageRenderer>()->GetImageSize().y + tlposition.y;	// 끝점을 찾아준다.

		float reyscale = ((float)step / 10) * yscale;											// 이미지의 크기를 계산
		bgm->GetComponent<Transform>()->SetScale(xscale, reyscale, 1.0f);		// 이미지의 크기를 지정
		blposition -= (bgm->GetComponent<ImageRenderer>()->GetImageSize().y * reyscale) + tlposition.y;
		float ypos = /*tlposition.y + */(tlposition.y + blposition);

		// X포지션
		float leftpos = bgm->GetComponent<ImageRenderer>()->GetImageSize().x;
		leftpos = ((leftpos* xscale) * step) + tlposition.x;

		//총합
		bgm->GetComponent<ImageRenderer>()->SetPosition(leftpos, ypos);
	}
}

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
