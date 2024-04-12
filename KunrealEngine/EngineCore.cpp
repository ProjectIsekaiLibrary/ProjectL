#include "EngineCore.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "GraphicsSystem.h"
#include "PhysicsSystem.h"
#include "InputSystem.h"
#include "SoundSystem.h"
#include "Navigation.h"
#include "ButtonSystem.h"

#include "Coroutine.h"

#include "ComponentHeaders.h"

/// 꼭 지워야해 내 수학라이브러리와 동일한 결과가 나오는지 체크용
#include <DirectXMath.h>
/// 

KunrealEngine::SceneManager& sceneInstance = KunrealEngine::SceneManager::GetInstance();
KunrealEngine::TimeManager& timeInstance = KunrealEngine::TimeManager::GetInstance();
KunrealEngine::InputSystem* inputInstance = KunrealEngine::InputSystem::GetInstance();
KunrealEngine::SoundSystem& soundInstance = KunrealEngine::SoundSystem::GetInstance();
KunrealEngine::Navigation& navigationInstance = KunrealEngine::Navigation::GetInstance();

KunrealEngine::GameObject* player;
KunrealEngine::GameObject* kamen;
KunrealEngine::GameObject* zeolight;
KunrealEngine::GameObject* zeolight_Image;
KunrealEngine::GameObject* Button_Image1;
KunrealEngine::GameObject* Button_Image2;
KunrealEngine::GameObject* Button_Image3;
KunrealEngine::GameObject* Button_Image4;

KunrealEngine::GameObject* testCamera;

DirectX::XMFLOAT3 targetPos;

KunrealEngine::EngineCore::EngineCore()
	:_gInterface(nullptr)
{

}

KunrealEngine::EngineCore::~EngineCore()
{

}

void KunrealEngine::EngineCore::Initialize(HWND hwnd, HINSTANCE hInstance, int screenWidth, int screenHeight)
{
	// Initailize
	sceneInstance.Initialize();
	timeInstance.Initialize();
	GraphicsSystem::GetInstance().Initialize(hwnd, screenWidth, screenHeight);
	PhysicsSystem::GetInstance().Initialize();
	inputInstance->Initialize(hInstance, hwnd, screenHeight, screenWidth);
	soundInstance.Initialize(hwnd);

	navigationInstance.Initialize();
	navigationInstance.HandleBuild();
	//NavigationInstance.LoadAll("Resources/Navimesh/Player_navmesh.bin", 0);
	navigationInstance.LoadAll("Resources/Navimesh/Player_navmesh.bin", 1);
	navigationInstance.LoadAll("Resources/Navimesh/Player_navmesh.bin", 2);
	navigationInstance.LoadAll("Resources/Navimesh/Player_navmesh.bin", 3);
	navigationInstance.LoadAll("Resources/Navimesh/Player_navmesh.bin", 4);
	navigationInstance.SetSEpos(0, -23.0f, 0.0f, -10.0f, -90.0f, 0.0f, 96.0f);
	navigationInstance.SetSEpos(1, -23.0f, 0.0f, -10.0f, -90.0f, 0.0f, 96.0f);

	navigationInstance.AddTempObstacle(DirectX::XMFLOAT3(-40.0f, 0.0f, -35.0f), 3.0f, 1.0f);

	std::vector<std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3>> navipos1 = navigationInstance.FindStraightPath(0);
	std::vector<std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3>> navipos2 = navigationInstance.FindStraightPath(1);
	DirectX::XMFLOAT3 navipos3 = navigationInstance.FindRaycastPath(0);

	for (auto path : navipos1)
	{
		GRAPHICS->CreateDebugLine(path.first, path.second, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
	}

	for (auto path : navipos2)
	{
		GRAPHICS->CreateDebugLine(path.first, path.second, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	GRAPHICS->CreateDebugLine(DirectX::XMFLOAT3(-23.0f, 0.0f, -10.0f), navipos3, DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f));


	/// 니들 맘대로 해
	PlayGround();
}

void KunrealEngine::EngineCore::Release()
{
	sceneInstance.Release();
	timeInstance.Release();
	KunrealEngine::GraphicsSystem::GetInstance().Release();
	/// 나머지들 release 할 것들 추가
}

void KunrealEngine::EngineCore::FixedUpdate()
{
	PhysicsSystem::GetInstance().FixedUpdate();
	sceneInstance.FixedUpdateScene(sceneInstance.GetCurrentScene());
}

bool moveTo = true;

void KunrealEngine::EngineCore::Update()
{
	inputInstance->Update(GetDeltaTime());
	inputInstance->UpdateEditorMousePos(_editorMousepos);
	sceneInstance.UpdateScene(sceneInstance.GetCurrentScene());
	GraphicsSystem::GetInstance().Update(_editorMousepos.x, _editorMousepos.y);
	navigationInstance.HandleUpdate(TimeManager::GetInstance().GetDeltaTime());

	CheckMousePosition();

	if (inputInstance->MouseButtonUp(1))
	{
		int a = 10;
	}

	//ninaveh->GetComponent<Animator>()->Play(0, 20.0f, true);

	GRAPHICS->DrawDebugText(100, 100, 20, "FPS : %.2f", 1 / TimeManager::GetInstance().GetDeltaTime());
	//GRAPHICS->DrawDebugText(100, 100, 20, "1610배마owner정기훈 : %f", sceneInstance.GetCurrentScene()->GetGameObject("modelTest")->GetComponent<Transform>()->GetPosition().x);
	//GRAPHICS->DrawDebugText(100, 300, 30, "%s", sceneInstance.GetCurrentScene()->GetSceneName().c_str());
	//GRAPHICS->DrawDebugText(500, 500, 30, "Player HP : %f", player->GetComponent<Player>()->GetPlayerData()._hp);

	// 물리 정보 교환 테스트
	BoxCollider* colA = sceneInstance.GetCurrentScene()->GetGameObject("Knife")->GetComponent<BoxCollider>();
	BoxCollider* colB = sceneInstance.GetCurrentScene()->GetGameObject("Rock")->GetComponent<BoxCollider>();
	BoxCollider* playerCol = sceneInstance.GetCurrentScene()->GetGameObject("Player")->GetComponent<BoxCollider>();

	Transform* trr = sceneInstance.GetCurrentScene()->GetGameObject("Knife")->GetComponent<Transform>();

	if (trr->GetPosition().x > 20.f)
	{
		moveTo = false;
	}

	if (trr->GetPosition().x < -20.f)
	{
		moveTo = true;
	}

	if (moveTo)
	{
		trr->SetPosition(trr->GetPosition().x + 0.11f, 5.f, 0.f);
	}
	else
	{
		trr->SetPosition(trr->GetPosition().x - 0.11f, 5.f, 0.f);
	}
	//sceneInstance.GetCurrentScene()->GetGameObject("Knife")->GetComponent<Transform>()->SetPosition(sceneInstance.GetCurrentScene()->GetGameObject("Knife")->GetComponent<Transform>()->GetPosition().x + 0.1f, 0.f, 0.f);

	//sceneInstance.GetCurrentScene()->GetGameObject("Rock")->GetComponent<Transform>()->SetPosition(sceneInstance.GetCurrentScene()->GetGameObject("Rock")->GetComponent<Transform>()->GetPosition().x + 0.1f, 5.f, 0.f);



	if (playerCol->GetTargetObject() != nullptr)
	{
		//GRAPHICS->DrawDebugText(800, 50, 60, "%s%s", "TargetObj : ", playerCol->GetTargetObject()->GetObjectName().c_str());
	}
	else
	{
		//GRAPHICS->DrawDebugText(800, 50, 60, "%s", "TargetObj : None");
	}

	auto x = testCamera->GetComponent<Transform>()->GetPosition().x;
	auto y = testCamera->GetComponent<Transform>()->GetPosition().y;
	auto z = testCamera->GetComponent<Transform>()->GetPosition().z;


	if (inputInstance->KeyInput(KEY::B))
	{
		kamen->GetComponent<BoxCollider>()->SeperateFromParent();
	}
	else if (inputInstance->KeyInput(KEY::N))      
	{
		kamen->GetComponent<BoxCollider>()->SetTransform(kamen, "mixamorig:RightHand");
	}
	//else if (inputInstance->KeyInput(KEY::A))
	//{
	//	testCamera->GetComponent<KunrealEngine::Camera>()->SetCameraPosition(-20.0f * TimeManager::GetInstance().GetDeltaTime() + x, y, z);
	//}
	//else if (inputInstance->KeyInput(KEY::D))
	//{
	//	testCamera->GetComponent<KunrealEngine::Camera>()->SetCameraPosition(20.0f * TimeManager::GetInstance().GetDeltaTime() + x, y, z);
	//}
	//else if (inputInstance->KeyInput(KEY::W))
	//{
	//	testCamera->GetComponent<KunrealEngine::Camera>()->SetCameraPosition(x, y, z + 20.0f * TimeManager::GetInstance().GetDeltaTime());
	//}
	//else if (inputInstance->KeyInput(KEY::S))
	//{
	//	testCamera->GetComponent<KunrealEngine::Camera>()->SetCameraPosition(x, y, z - 20.0f * TimeManager::GetInstance().GetDeltaTime());
	//}
	//else if (inputInstance->KeyInput(KEY::Q))
	//{
	//	testCamera->GetComponent<KunrealEngine::Camera>()->SetCameraPosition(x, y - 20.0f * TimeManager::GetInstance().GetDeltaTime(), z);
	//}
	//else if (inputInstance->KeyInput(KEY::E))
	//{
	//	testCamera->GetComponent<KunrealEngine::Camera>()->SetCameraPosition(x, y + 20.0f * TimeManager::GetInstance().GetDeltaTime(), z);
	//}
	//else if (inputInstance->KeyInput(KEY::R))
	//{
	//	GRAPHICS->GetMainCamera()->RotateCamera({ 0,  -20.0f * TimeManager::GetInstance().GetDeltaTime() });
	//}
	//else if (inputInstance->KeyInput(KEY::T))
	//{
	//	GRAPHICS->GetMainCamera()->RotateCamera({ 0,  20.0f * TimeManager::GetInstance().GetDeltaTime() });
	//}


	SoundSystem::GetInstance().updateListenerPosition(x,y,z);

	if (inputInstance->KeyDown(KEY::LBRACKET))
	{
		zeolight->GetComponent<SoundPlayer>()->Change3Dmode(0);
	}
	else if (inputInstance->KeyDown(KEY::RBRACKET))
	{
		zeolight->GetComponent<SoundPlayer>()->RemoveSound(0);
	}
	else if (inputInstance->KeyDown(KEY::_1))
	{
		zeolight->GetComponent<SoundPlayer>()->Play(0);
	}
	else if (inputInstance->KeyDown(KEY::_2))
	{
		zeolight->GetComponent<SoundPlayer>()->Play(1);
	}
	else if (inputInstance->KeyDown(KEY::_3))
	{
		zeolight->GetComponent<SoundPlayer>()->Play(2);
	}
	else if (inputInstance->KeyDown(KEY::O))
	{
		zeolight->GetComponent<SoundPlayer>()->StopAll();
	}

	inputInstance->GetMousePosition(_ingameMouseX, _ingameMouseY);

	cursorimage->SetPosition(_ingameMouseX, _ingameMouseY);
	cursorimage->SetScale(0.9 * 0.1, 1.6 * 0.1);

	// 플레이어 상태
	if (player->GetComponent<Player>()->GetPlayerStatus() == Player::Status::IDLE)
	{
		//GRAPHICS->DrawDebugText(200, 300, 20, "Player Status : IDLE");
	}
	else if (player->GetComponent<Player>()->GetPlayerStatus() == Player::Status::WALK)
	{
		//GRAPHICS->DrawDebugText(200, 300, 20, "Player Status : WALK");
	}
	else if (player->GetComponent<Player>()->GetPlayerStatus() == Player::Status::DASH)
	{
		//GRAPHICS->DrawDebugText(200, 300, 20, "Player Status : DASH");
	}
	else
	{
		//GRAPHICS->DrawDebugText(200, 300, 20, "Player Status : Idontknow");
	}

	Updatecoroutine();


	// 플레이어 상태
	if (player->GetComponent<Player>()->GetPlayerStatus() == Player::Status::IDLE)
	{
		//GRAPHICS->DrawDebugText(200, 300, 20, "Player Status : IDLE");
	}
	else if (player->GetComponent<Player>()->GetPlayerStatus() == Player::Status::WALK)
	{
		//GRAPHICS->DrawDebugText(200, 300, 20, "Player Status : WALK");
	}
	else if (player->GetComponent<Player>()->GetPlayerStatus() == Player::Status::DASH)
	{
		//GRAPHICS->DrawDebugText(200, 300, 20, "Player Status : DASH");
	}
	else
	{
		//GRAPHICS->DrawDebugText(200, 300, 20, "Player Status : Idontknow");
	}

	switch (kamen->GetComponent<Kamen>()->GetStatus())
	{
		case BossStatus::ENTER:
		{
			GRAPHICS->DrawDebugText(200, 350, 20, "Boss Status : ENTER");
			break;
		}
		case BossStatus::IDLE:
		{
			GRAPHICS->DrawDebugText(200, 350, 20, "Boss Status : IDLE");
			break;
		}
		case BossStatus::CHASE:
		{
			GRAPHICS->DrawDebugText(200, 350, 20, "Boss Status : CHASE");
			break;
		}
		case BossStatus::STAGGERED:
		{
			GRAPHICS->DrawDebugText(200, 350, 20, "Boss Status : ON_STAGGERED");
			break;
		}
		case BossStatus::OFF_STAGGRED:
		{
			GRAPHICS->DrawDebugText(200, 350, 20, "Boss Status : OFF_STAGGERED");
			break;
		}
		case BossStatus::DEAD:
		{
			GRAPHICS->DrawDebugText(200, 350, 20, "Boss Status : DEAD");
			break;
		}
		case BossStatus::BASIC_ATTACK:
		{
			GRAPHICS->DrawDebugText(200, 350, 20, "Boss Status : BASIC_ATTACK");
			break;
		}
		case BossStatus::CORE_ATTACK:
		{
			GRAPHICS->DrawDebugText(200, 350, 20, "Boss Status : CORE_ATTACK");
			break;
		}
		case BossStatus::PATTERN_END:
		{
			GRAPHICS->DrawDebugText(200, 350, 20, "Boss Status : PATTERN_END");
			break;
		}
		default:
			break;
	}
}

void KunrealEngine::EngineCore::LateUpdate()
{
	sceneInstance.LateUpdateScene(sceneInstance.GetCurrentScene());
	GRAPHICS->Update();	// 그래픽스쪽 카메라 업데이트 관련
}

void KunrealEngine::EngineCore::UpdateAll()
{
	timeInstance.Update();
	this->FixedUpdate();
	this->Update();
	this->LateUpdate();
	this->Render();
}

void KunrealEngine::EngineCore::Render()
{
	GRAPHICS->Render();
}

void KunrealEngine::EngineCore::SetEditorMousePos(POINT position)
{
	_editorMousepos = position;
	return;
}

void KunrealEngine::EngineCore::PlayGround()
{
	// Camera
	KunrealEngine::KunrealMath::Float3 cameraPos = { 0.0f, 30.0f, -55.0f };
	// KunrealEngine::KunrealMath::Float3 cameraPos = { 40.0f, 2.0f, -30.0f };
	KunrealEngine::KunrealMath::Float3 targetPos = { 0.0f, -15.0f, 0.0f };
	testCamera = sceneInstance.GetCurrentScene()->CreateObject("testCamera");
	testCamera->AddComponent<Camera>();
	testCamera->GetComponent<Camera>()->SetCameraPosition(cameraPos.x, cameraPos.y, cameraPos.z);
	testCamera->GetComponent<Camera>()->SetTargetPosition(targetPos.x, targetPos.y, targetPos.z);
	//testCamera->GetComponent<Camera>()->RotateCamera(5.f, 0.f);
	testCamera->GetComponent<Camera>()->SetMainCamera();
	
	//testCamera->GetComponent<Transform>()->SetPosition(-32.f, 45.f, -32.f);
	//testCamera->GetComponent<Transform>()->SetRotation(0.f, 45.f, 0.f);

	// Player
	player = sceneInstance.GetCurrentScene()->CreateObject("Player");
	player->AddComponent<MeshRenderer>();
	player->GetComponent<MeshRenderer>()->SetMeshObject("PlayerWithCloak/PlayerWithCloak");
	player->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	player->GetComponent<Transform>()->SetRotation(0.0f, 45.f, 0.0f);
	player->GetComponent<MeshRenderer>()->SetActive(true);
	player->GetComponent<MeshRenderer>()->SetPickableState(true);
	KunrealEngine::KunrealMath::Float4 reflect = { 0.9f, 0.9f, 0.9f, 0.9f };
	//sceneInstance.GetCurrentScene()->GetGameObject("modelTest")->GetComponent<MeshRenderer>()->SetDiffuseTexture("Resources/Textures/Kachujin/Kachujin_diffuse.png");
	sceneInstance.GetCurrentScene()->GetGameObject("Player")->GetComponent<MeshRenderer>()->SetReflect(reflect.x, reflect.y, reflect.z, reflect.w);
	player->AddComponent<BoxCollider>();
	player->GetComponent<BoxCollider>()->SetBoxSize(3.0f, 8.0f, 5.0f);

	player->AddComponent<Player>();

	kamen = sceneInstance.GetCurrentScene()->CreateObject("kamen");
	kamen->AddComponent<Kamen>();
	
	// 워프용 비석_이미지
	zeolight_Image = sceneInstance.GetCurrentScene()->CreateObject("zeolight_Image");
	zeolight_Image->AddComponent<ImageRenderer>();
	zeolight_Image->GetComponent<ImageRenderer>()->SetImage("BackGround.png");
	zeolight_Image->GetComponent<ImageRenderer>()->SetImageStatus(false);
	zeolight_Image->GetComponent<ImageRenderer>()->SetPosition(0.f, 0.f);
	zeolight_Image->GetComponent<Transform>()->SetScale(10.0f, 10.0f, 1.0f);

	Button_Image1 = sceneInstance.GetCurrentScene()->CreateObject("Button_Image1");
	Button_Image1->SetParent(zeolight_Image);
	Button_Image1->AddComponent<ImageRenderer>();
	Button_Image1->GetComponent<ImageRenderer>()->SetImage("Stage1_test.png");
	Button_Image1->GetComponent<ImageRenderer>()->SetImageStatus(false);
	Button_Image1->GetComponent<ImageRenderer>()->SetPosition(300.f, 300.f);
	Button_Image1->GetComponent<Transform>()->SetScale(2.0f, 2.0f, 1.0f);
	Button_Image1->AddComponent<ButtonSystem>();
	Button_Image1->GetComponent<ButtonSystem>()->SetImage(Button_Image1->GetComponent<ImageRenderer>());

	Button_Image2 = sceneInstance.GetCurrentScene()->CreateObject("Button_Image2");
	Button_Image2->SetParent(zeolight_Image);
	Button_Image2->AddComponent<ImageRenderer>();
	Button_Image2->GetComponent<ImageRenderer>()->SetImage("Stage2_test.png");
	Button_Image2->GetComponent<ImageRenderer>()->SetImageStatus(false);
	Button_Image2->GetComponent<ImageRenderer>()->SetPosition(300.f, 600.f);
	Button_Image2->GetComponent<Transform>()->SetScale(2.0f, 2.0f, 1.0f);
	Button_Image2->AddComponent<ButtonSystem>();
	Button_Image2->GetComponent<ButtonSystem>()->SetImage(Button_Image2->GetComponent<ImageRenderer>());

	Button_Image3 = sceneInstance.GetCurrentScene()->CreateObject("Button_Image3");
	Button_Image3->SetParent(zeolight_Image);
	Button_Image3->AddComponent<ImageRenderer>();
	Button_Image3->GetComponent<ImageRenderer>()->SetImage("Stage3_test.png");
	Button_Image3->GetComponent<ImageRenderer>()->SetImageStatus(false);
	Button_Image3->GetComponent<ImageRenderer>()->SetPosition(1200.f, 300.f);
	Button_Image3->GetComponent<Transform>()->SetScale(2.0f, 2.0f, 1.0f);
	Button_Image3->AddComponent<ButtonSystem>();
	Button_Image3->GetComponent<ButtonSystem>()->SetImage(Button_Image3->GetComponent<ImageRenderer>());

	Button_Image4 = sceneInstance.GetCurrentScene()->CreateObject("Button_Image4");
	Button_Image4->SetParent(zeolight_Image);
	Button_Image4->AddComponent<ImageRenderer>();
	Button_Image4->GetComponent<ImageRenderer>()->SetImage("Stage4_test.png");
	Button_Image4->GetComponent<ImageRenderer>()->SetImageStatus(false);
	Button_Image4->GetComponent<ImageRenderer>()->SetPosition(1200.f, 600.f);
	Button_Image4->GetComponent<Transform>()->SetScale(2.0f, 2.0f, 1.0f);
	Button_Image4->AddComponent<ButtonSystem>();
	Button_Image4->GetComponent<ButtonSystem>()->SetImage(Button_Image4->GetComponent<ImageRenderer>());

	// 워프용 비석
	zeolight = sceneInstance.GetCurrentScene()->CreateObject("NPC_zeolight");
	zeolight->AddComponent<MeshRenderer>();
	zeolight->GetComponent<MeshRenderer>()->SetMeshObject("cube/cube");
	zeolight->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 0.0f);
	zeolight->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);
	zeolight->GetComponent<Transform>()->SetRotation(0.f, 0.f, 0.f);
		//	충돌판정(?)을 위한 콜라이더 컴포넌트
	zeolight->AddComponent<BoxCollider>();
	zeolight->GetComponent<BoxCollider>()->SetBoxSize(1.0f, 1.0f, 1.0f);
		//	NPC로써 가지고 있어야할 컴포넌트
	zeolight->AddComponent<Zeolight>();
	zeolight->GetComponent<Zeolight>()->SetInteractionRange(10);
		// 워프용 비석에 필요한 부분은 아님. 그냥 사운드 테스트용으로 붙은거.
	zeolight->AddComponent<SoundPlayer>();
	zeolight->GetComponent<SoundPlayer>()->CreateSoundInfo("Resources/Sound/An_die_Freude.wav",true,true);
	zeolight->GetComponent<SoundPlayer>()->CreateSound(0);
	zeolight->GetComponent<SoundPlayer>()->CreateSoundInfo("Resources/Sound/Keyboard_typing.wav",true,true);
	zeolight->GetComponent<SoundPlayer>()->CreateSound(1);
	zeolight->GetComponent<SoundPlayer>()->CreateSoundInfo("Resources/Sound/soundch1.wav",true,true);
	zeolight->GetComponent<SoundPlayer>()->CreateSound(2);
	KunrealEngine::SoundSystem::GetInstance().GetSoundPathList();


	// Plane 
	auto plane = sceneInstance.GetCurrentScene()->CreateObject("plane");
	plane->AddComponent<MeshRenderer>();
	plane->GetComponent<MeshRenderer>()->SetMeshObject("cube/cube", true);
	plane->GetComponent<MeshRenderer>()->SetDiffuseTexture(0, "floor.dds");
	//plane->GetComponent<MeshRenderer>()->SetNormalTexture(0, "floor_nmap.dds");
	plane->GetComponent<Transform>()->SetScale(100.0f, 1.0f, 100.0f);
	plane->GetComponent<Transform>()->SetPosition(0, -1.0f, 0);
	plane->GetComponent<MeshRenderer>()->SetShadowState(false);

	//// cube map test
	GRAPHICS->CreateCubeMap("test", "sunsetcube1024.dds", true);
	auto list = GRAPHICS->GetCubeMapList();
	GRAPHICS->SetMainCubeMap(list.back());

	// light test
	DirectX::XMFLOAT4 diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT4 ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	DirectX::XMFLOAT4 specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 direction = { -0.5f, -0.5f, 0.5f };

	auto lightTest = sceneInstance.GetCurrentScene()->CreateObject("DirectionalLight");
	lightTest->AddComponent<Light>();
	lightTest->GetComponent<Light>()->CreateDirectionalLight(ambient, diffuse, specular, direction);
	lightTest->GetComponent<Light>()->SetActive(true);
	lightTest->SetActive(true);

	// PointLight 예시
	auto pointLight = sceneInstance.GetCurrentScene()->CreateObject("pointLightTest");
	pointLight->AddComponent<Light>();
	pointLight->GetComponent<Transform>()->SetPosition(0.f, 0.f, 0.f);
	pointLight->GetComponent<Light>()->CreatePointLight(ambient, diffuse, specular, 1.0f);
	pointLight->GetComponent<Light>()->SetAmbient(1.0f, 1.0f, 1.0f, 1.0f);
	pointLight->GetComponent<Light>()->SetPointRange(10.f);
	pointLight->GetComponent<Light>()->SetPointRange(100.0f);
	pointLight->SetActive(false);

	auto pointLight2 = sceneInstance.GetCurrentScene()->CreateObject("pointLightTest2");
	pointLight2->AddComponent<Light>();
	pointLight2->GetComponent<Transform>()->SetPosition(-70.f, 0.f, 50.f);
	pointLight2->GetComponent<Light>()->CreatePointLight(ambient, diffuse, specular, 0.0f);
	pointLight2->GetComponent<Light>()->SetAmbient(1.0f, 1.0f, 1.0f, 1.0f);
	pointLight2->GetComponent<Light>()->SetPointRange(50.f);
	pointLight2->SetActive(false);

	// 충돌체크
	auto c1 = sceneInstance.GetCurrentScene()->CreateObject("Knife");
	auto c2 = sceneInstance.GetCurrentScene()->CreateObject("Rock");


	c1->AddComponent<BoxCollider>();
	c1->GetComponent<BoxCollider>()->SetBoxSize(5.f, 10.f, 10.f);
	c1->AddComponent<MeshRenderer>();
	c1->GetComponent<Transform>()->SetPosition(-20.f, 9.f, 0.f);
	c1->GetComponent<Transform>()->SetScale(0.3f, 0.3f, 0.3f);
	c1->GetComponent<MeshRenderer>()->SetMeshObject("Sword/Sword", "bricks.dds");
	c1->GetComponent<MeshRenderer>()->SetPickableState(true);

	c2->AddComponent<BoxCollider>();
	c2->GetComponent<BoxCollider>()->SetBoxSize(11.f, 11.f, 11.f);
	c2->AddComponent<MeshRenderer>();
	c2->GetComponent<Transform>()->SetPosition(-40.f, 15.f, 0.f);
	c2->GetComponent<Transform>()->SetScale(5.f, 5.f, 5.f);
	//c2->GetComponent<MeshRenderer>()->SetMeshObject("Kachujin/Kachujin", "Kachujin/Kachujin_diffuse.png");
	c2->GetComponent<MeshRenderer>()->SetMeshObject("cube/cube", "bricks.dds");
	c2->GetComponent<MeshRenderer>()->SetPickableState(true);

	targetPos = { 0.0f ,0.0f ,0.0f };

	auto imageTest = sceneInstance.GetCurrentScene()->CreateObject("ImageTest1");
	imageTest->AddComponent<ImageRenderer>();
	imageTest->GetComponent<ImageRenderer>()->SetImage("MenuQuitButton.dds");
	imageTest->GetComponent<ImageRenderer>()->SetImageStatus(true);
	imageTest->GetComponent<ImageRenderer>()->SetPosition(500.f, 400.f);
	imageTest->GetComponent<ImageRenderer>()->SetImageStatus(false);

	cursorimage = GRAPHICS->CreateImage("Resources/Textures/floor.dds");
	cursorimage->SetPosition(500.0f);
	cursorimage->SetRenderingState(false);

	GRAPHICS->CreateDebugLine(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT3(100.0f, 1.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
	//GRAPHICS->DeleteAllLine();
	// Test
	for (int i = 0; i < 10; i++)
	{
		std::string name = "rightCube" + std::to_string(i);
		auto cube1 = sceneInstance.GetCurrentScene()->CreateObject(name);
		cube1->AddComponent<MeshRenderer>();
		cube1->GetComponent<MeshRenderer>()->SetMeshObject("cube/cube", true);
		cube1->GetComponent<MeshRenderer>()->SetDiffuseTexture(0, "bricks.dds");
		cube1->GetComponent<MeshRenderer>()->SetNormalTexture(0, "bricks_nmap.dds");
		cube1->GetComponent<Transform>()->SetScale(2.0f, 2.0f, 2.0f);
		cube1->GetComponent<Transform>()->SetPosition(40.0f, 2.0f, 50.0f - 10.0f * i);

		if (i % 2 == 0)
		{
			cube1->GetComponent<MeshRenderer>()->SetPickableState(true);
		}
	}

	for (int i = 0; i < 10; i++)
	{
		std::string name = "leftCube" + std::to_string(i);
		auto cube1 = sceneInstance.GetCurrentScene()->CreateObject(name);
		cube1->AddComponent<MeshRenderer>();
		cube1->GetComponent<MeshRenderer>()->SetMeshObject("cube/cube", true);
		cube1->GetComponent<MeshRenderer>()->SetDiffuseTexture(0, "bricks.dds");
		//cube1->GetComponent<MeshRenderer>()->SetNormalTexture("bricks_nmap.dds");
		cube1->GetComponent<Transform>()->SetScale(2.0f, 2.0f, 2.0f);
		cube1->GetComponent<Transform>()->SetPosition(-40.0f, 2.0f, 50.0f - 10.0f * i);

		if (i % 2 == 1)
		{
			cube1->GetComponent<MeshRenderer>()->SetPickableState(true);
		}
	}

	GameObject* asdfgqwea = GetCurrentScene()->CreateObject("asdfgqwert");
	asdfgqwea->AddComponent<MeshRenderer>();
}

void KunrealEngine::EngineCore::CheckMousePosition()
{
	// 에디터로 실행하지 않았을 경우
	if (_editorMousepos.x == -1 && _editorMousepos.y == -1)
	{
		{
			_finalMousePosition.x = _ingameMouseX;
			_finalMousePosition.y = _ingameMouseY;
		}
	}

	else
	{
		_finalMousePosition = _editorMousepos;
	}
}

float KunrealEngine::EngineCore::GetDeltaTime()
{
	return timeInstance.GetDeltaTime();
}
