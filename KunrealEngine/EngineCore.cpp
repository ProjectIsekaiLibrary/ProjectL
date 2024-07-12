#include "EngineCore.h"
#include <DirectXMath.h>
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
#include "EventManager.h"
#include "ComponentHeaders.h"
#include "UIPackageFuntion.hpp"
#include "ToolBox.h"
#include "TimeManager.h"

KunrealEngine::SceneManager& sceneInstance = KunrealEngine::SceneManager::GetInstance();
KunrealEngine::TimeManager& timeInstance = KunrealEngine::TimeManager::GetInstance();
KunrealEngine::InputSystem* inputInstance = KunrealEngine::InputSystem::GetInstance();
KunrealEngine::SoundSystem& soundInstance = KunrealEngine::SoundSystem::GetInstance();
KunrealEngine::Navigation& navigationInstance = KunrealEngine::Navigation::GetInstance();
KunrealEngine::EventManager& eventInstance = KunrealEngine::EventManager::GetInstance();

KunrealEngine::GameObject* player;
KunrealEngine::GameObject* kamen;
KunrealEngine::GameObject* spider;
KunrealEngine::GameObject* zeolight;
KunrealEngine::GameObject* bossMap;
KunrealEngine::GameObject* tree2;
KunrealEngine::GameObject* tree3;
KunrealEngine::GameObject* tree4;
KunrealEngine::GameObject* titleRock1;
KunrealEngine::GameObject* titleRock2;
KunrealEngine::GameObject* titleRock3;
KunrealEngine::GameObject* floatingObj;

KunrealEngine::GameObject* Title_ui_box;	// 타이틀UI
KunrealEngine::GameObject* pause_ui_box;	// 일시정지
KunrealEngine::GameObject* option_ui_box;	// 일시정지
KunrealEngine::GameObject* battle_ui_box;	// 전투UI

KunrealEngine::GameObject* particle;
KunrealEngine::GameObject* particle2;
KunrealEngine::GameObject* particle3;
KunrealEngine::GameObject* particle4;
KunrealEngine::GameObject* particle5;
KunrealEngine::GameObject* particle6;
KunrealEngine::GameObject* particle7;
KunrealEngine::GameObject* particle8;
KunrealEngine::GameObject* particle9;
KunrealEngine::GameObject* particle10;
KunrealEngine::GameObject* particle11;
KunrealEngine::GameObject* particle12;
KunrealEngine::GameObject* particle13;
KunrealEngine::GameObject* particle14;
KunrealEngine::GameObject* particle15;
KunrealEngine::GameObject* particle16;

KunrealEngine::GameObject* particle17;
KunrealEngine::GameObject* particle18;
KunrealEngine::GameObject* particle19;
KunrealEngine::GameObject* particle20;
KunrealEngine::GameObject* particle21;
KunrealEngine::GameObject* particle22;
KunrealEngine::GameObject* particle23;
KunrealEngine::GameObject* particle24;

KunrealEngine::GameObject* particle25;
KunrealEngine::GameObject* particle26;
KunrealEngine::GameObject* particle27;
KunrealEngine::GameObject* particle28;
KunrealEngine::GameObject* particle29;
KunrealEngine::GameObject* particle30;
KunrealEngine::GameObject* particle31;
KunrealEngine::GameObject* particle32;


KunrealEngine::GameObject* particlePlayerQ1;
KunrealEngine::GameObject* particlePlayerQ2;
KunrealEngine::GameObject* particlePlayerQ3;
KunrealEngine::GameObject* particlePlayerQ4;

KunrealEngine::GameObject* particlePlayerW1;
KunrealEngine::GameObject* particlePlayerW2;

KunrealEngine::GameObject* particlePlayerR1;
KunrealEngine::GameObject* particlePlayerR2;
KunrealEngine::GameObject* particlePlayerR3;
KunrealEngine::GameObject* particlePlayerR4;
KunrealEngine::GameObject* particlePlayerR5;

KunrealEngine::GameObject* particlePortal1;
KunrealEngine::GameObject* particlePortal2;
KunrealEngine::GameObject* particlePortal3;

KunrealEngine::GameObject* particleBossCall1;
KunrealEngine::GameObject* particleBossCall2;
KunrealEngine::GameObject* particleBossCall3;

KunrealEngine::GameObject* particleBossPillar1;
KunrealEngine::GameObject* particleBossPillar2;
KunrealEngine::GameObject* particleBossPillar3;

KunrealEngine::GameObject* particleBossShot1;
KunrealEngine::GameObject* particleBossShot2;

KunrealEngine::GameObject* particleBossSword1;
KunrealEngine::GameObject* particleBossSword2;
KunrealEngine::GameObject* particleBossSword3;
KunrealEngine::GameObject* particleBossSword4;

KunrealEngine::GameObject* particleBossSword2_1;
KunrealEngine::GameObject* particleBossSword2_2;
KunrealEngine::GameObject* particleBossSword2_3;
KunrealEngine::GameObject* particleBossSword2_4;

KunrealEngine::GameObject* particleBossDonut1;
KunrealEngine::GameObject* particleBossDonut2;
KunrealEngine::GameObject* particleBossDonut3;
KunrealEngine::GameObject* particleBossDonut4;

std::vector<KunrealEngine::GameObject*> ParticleSwordSoulList;

KunrealEngine::GameObject* particleBezierTest1;
KunrealEngine::GameObject* particleBezierTest2;
KunrealEngine::GameObject* particleBezierTest3;

KunrealEngine::GameObject* playerE1;
KunrealEngine::GameObject* playerE2;
KunrealEngine::GameObject* playerE3;

KunrealEngine::GameObject* playerE2_1;
KunrealEngine::GameObject* playerE2_2;
KunrealEngine::GameObject* playerE2_3;
KunrealEngine::GameObject* playerE2_4;

KunrealEngine::GameObject* bossTeleportAttack1;
KunrealEngine::GameObject* bossTeleportAttack2;
KunrealEngine::GameObject* bossTeleportAttack3;
KunrealEngine::GameObject* bossTeleportAttack4;
KunrealEngine::GameObject* bossTeleportAttack5;

KunrealEngine::GameObject* testCamera;
KunrealEngine::GameObject* particleCamera;

KunrealEngine::GameObject* playerQEnd1;
KunrealEngine::GameObject* playerQEnd2;
KunrealEngine::GameObject* playerQEnd3;
KunrealEngine::GameObject* playerQEnd4;

KunrealEngine::GameObject* playerREnd1;
KunrealEngine::GameObject* playerREnd2;
KunrealEngine::GameObject* playerREnd3;
KunrealEngine::GameObject* playerREnd4;

KunrealEngine::GameObject* mapParticle1;
KunrealEngine::GameObject* mapParticle2;
KunrealEngine::GameObject* mapParticle3;
KunrealEngine::GameObject* mapParticle4;
KunrealEngine::GameObject* mapParticle5;
KunrealEngine::GameObject* mapParticle6;
KunrealEngine::GameObject* mapParticle7;
KunrealEngine::GameObject* mapParticle8;

KunrealEngine::GameObject* mapParticleEye1;
KunrealEngine::GameObject* mapParticleEye2;
KunrealEngine::GameObject* mapParticleEye3;
KunrealEngine::GameObject* mapParticleEye4;
KunrealEngine::GameObject* mapParticleEye5;
KunrealEngine::GameObject* mapParticleEye6;
KunrealEngine::GameObject* mapParticleEye7;
KunrealEngine::GameObject* mapParticleEye8;

KunrealEngine::GameObject* bossEgoRush1;
KunrealEngine::GameObject* bossEgoRush2;
KunrealEngine::GameObject* bossEgoRush3;
KunrealEngine::GameObject* bossEgoRush4;

KunrealEngine::GameObject* boss3Phase1;
KunrealEngine::GameObject* boss3Phase2;
KunrealEngine::GameObject* boss3Phase3;
KunrealEngine::GameObject* boss3Phase2_2;
KunrealEngine::GameObject* boss3Phase3_2;

KunrealEngine::GameObject* bossCoreBladeMove1;
KunrealEngine::GameObject* bossCoreBladeMove2;
KunrealEngine::GameObject* bossCoreBladeMove3;
KunrealEngine::GameObject* bossCoreBladeMove4;


DirectX::XMFLOAT3 targetPos;
DirectX::XMFLOAT3 p0 = { 0, 0, 0 };
DirectX::XMFLOAT3 p1 = { 0, 0, 0 };
DirectX::XMFLOAT3 p2 = { 0, 0, 0 };
DirectX::XMFLOAT3 p3 = { 0, 0, 0 };

KunrealEngine::EngineCore::EngineCore()
	:_gInterface(nullptr), _isEditor(true)
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

	GameObject* meshTest = sceneInstance.GetCurrentScene()->CreateObject("meshTest");
	meshTest->AddComponent<MeshRenderer>();
	meshTest->GetComponent<MeshRenderer>()->SetMeshObject("cylinder/cylinder");
	meshTest->GetComponent<MeshRenderer>()->SetMeshObject("Blade/Blade");
	sceneInstance.GetCurrentScene()->DeleteGameObject(meshTest);

	PhysicsSystem::GetInstance().Initialize();
	inputInstance->Initialize(hInstance, hwnd, screenHeight, screenWidth);
	soundInstance.Initialize(hwnd);

	navigationInstance.Initialize();

	//// cube map test
	GRAPHICS->CreateCubeMap("test", "Texture6.dds", true);
	auto list = GRAPHICS->GetCubeMapList();
	GRAPHICS->SetMainCubeMap(list.back());

	sceneInstance.CreateScene("ParticleTest");

	_timeCount = 0.0f;
	_timeCountPlayerR = 0.0f;
	_isSettingTimer = false;
	_isBezierStartSetting = false;
	_isBezierBoomSetting = false;
	_isBezierTeleportSetting = false;

	//ChangeScene("ParticleTest");
	//ParticleTest();
	/// 니들 맘대로 해
	PlayGround();
	CreateTitleScene();
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
	//UpdateParticleTest();

	inputInstance->Update(GetDeltaTime());
	sceneInstance.UpdateScene(sceneInstance.GetCurrentScene());
	CheckMousePosition();

	navigationInstance.HandleUpdate(TimeManager::GetInstance().GetDeltaTime());
	soundInstance.Update();

	// UI 실행,종료
	if (inputInstance->KeyDown(KEY::ESCAPE))
	{
		pause_ui_box->SetActive(true);
	}

	if (inputInstance->KeyDown(KEY::PERIOD))
	{
		EventManager::GetInstance()._iscamfollow = EventManager::GetInstance()._iscamfollow ? false : true;
	}
	//GRAPHICS->DrawDebugText(100, 100, 20, "FPS : %.2f", 1 / TimeManager::GetInstance().GetDeltaTime());

	inputInstance->GetMousePosition(_ingameMouseX, _ingameMouseY);

	if (sceneInstance.GetCurrentScene()->GetSceneName() == "Title")
	{
		FloatingY(titleRock1, 5);
		FloatingY(titleRock2, 3);
		FloatingY(titleRock3, 4);
		FloatingY(floatingObj, 6);

		ShiveringLight(titleRock1);
		ShiveringLight(titleRock2);
		ShiveringLight(titleRock3);
	}
	MoveToMain();
	Updatecoroutine();
}

void KunrealEngine::EngineCore::LateUpdate()
{
	sceneInstance.LateUpdateScene(sceneInstance.GetCurrentScene());
	GRAPHICS->Update();	// 그래픽스쪽 카메라 업데이트 관련

	PhysicsSystem::GetInstance().PlayerForceUpdate();
}

void KunrealEngine::EngineCore::UpdateAll()
{
	timeInstance.Update();
	eventInstance.Update();
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
	navigationInstance.HandleBuild(0, "bossmap.obj");
	navigationInstance.HandleBuild(1, "bossmap.obj");

	// Camera
	DirectX::XMFLOAT3 cameraPos = { 0.0f, 0.0f, 1.0f };
	// KunrealEngine::KunrealMath::Float3 cameraPos = { 40.0f, 2.0f, -30.0f };
	DirectX::XMFLOAT3 targetPos = { 0.0f, 0.0f, 0.0f };
	testCamera = sceneInstance.GetCurrentScene()->CreateObject("testCamera");
	testCamera->AddComponent<Camera>();
	testCamera->GetComponent<Camera>()->SetCameraPosition(cameraPos.x, cameraPos.y, cameraPos.z);
	testCamera->GetComponent<Camera>()->SetTargetPosition(targetPos.x, targetPos.y, targetPos.z);
	//testCamera->GetComponent<Camera>()->RotateCamera(5.f, 0.f);
	testCamera->GetComponent<Camera>()->SetMainCamera();

	testCamera->GetComponent<Transform>()->SetPosition({ 0.0f, 120.0f, -130.0f });
	testCamera->GetComponent<Transform>()->SetRotation(-43.f, 180.f, 0.f);
	testCamera->_autoAwake = true;

	EventManager::GetInstance().SetCamera("testCamera");

	// Player
	player = sceneInstance.GetCurrentScene()->CreateObject("Player");
	player->AddComponent<Player>();
	player->_autoAwake = true;


	// Kamen
	kamen = sceneInstance.GetCurrentScene()->CreateObject("kamen");
	kamen->AddComponent<Kamen>();
	kamen->_autoAwake = true;

	// UI의 부모가 될 0,0pos객체
	battle_ui_box = sceneInstance.GetCurrentScene()->CreateObject("BattleUI");
	battle_ui_box->AddComponent<BattleUIManager>();
	battle_ui_box->_autoAwake = true;

	pause_ui_box = MakeMenuUIPack();
	option_ui_box = sceneInstance.GetCurrentScene()->CreateObject("Option");
	option_ui_box->AddComponent<OptionUIManager>();
	//Title_ui_box = 

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
	lightTest->_autoAwake = true;

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

	bossMap = sceneInstance.GetCurrentScene()->CreateObject("bossMap");
	bossMap->AddComponent<MeshRenderer>();
	bossMap->GetComponent<MeshRenderer>()->SetMeshObject("MapMesh/MapMesh");
	bossMap->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	bossMap->_autoAwake = true;

	// 맵 꾸미기 파티클
	MapParticleSetting();
}

void KunrealEngine::EngineCore::CheckMousePosition()
{
	// 에디터로 실행하지 않았을 경우
	if (_editorMousepos.x == -1 && _editorMousepos.y == -1)
	{
		{
			_finalMousePosition.x = _ingameMouseX;
			_finalMousePosition.y = _ingameMouseY;

			this->_isEditor = false;
		}
	}

	else
	{
		_finalMousePosition = _editorMousepos;

		this->_isEditor = true;
	}

	if (this->_isEditor)
	{
		inputInstance->UpdateEditorMousePos(_editorMousepos);
		GraphicsSystem::GetInstance().Update(_editorMousepos.x, _editorMousepos.y);
	}
	else
	{
		inputInstance->UpdateEditorMousePos(_finalMousePosition);
		GraphicsSystem::GetInstance().Update(_finalMousePosition.x, _finalMousePosition.y);
	}
}

void KunrealEngine::EngineCore::MakeObstacle()
{
	DirectX::XMFLOAT3 targetPos = GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y);
	DirectX::XMFLOAT3 bmin = { 4, 4, 4 };
	DirectX::XMFLOAT3 bmax = { 4, 4, 4 };
	navigationInstance.AddBoxTempObstacle(targetPos, bmin, bmax);

	KunrealEngine::GameObject* obstacle;
	obstacle = sceneInstance.GetCurrentScene()->CreateObject("obstacle");
	obstacle->AddComponent<MeshRenderer>();
	obstacle->GetComponent<MeshRenderer>()->SetMeshObject("cube/cube");
	obstacle->GetComponent<MeshRenderer>()->SetPickableState(true);
	obstacle->GetComponent<Transform>()->SetPosition(targetPos.x, targetPos.y, targetPos.z);
	obstacle->GetComponent<Transform>()->SetScale(4.0f, 4.0f, 4.0f);
	obstacle->GetComponent<Transform>()->SetRotation(0.f, 0.f, 0.f);
}

void KunrealEngine::EngineCore::RemoveObstacle()
{
	KunrealEngine::GameObject* obstacle = GraphicsSystem::GetInstance().GetPickedObject();
	DirectX::XMFLOAT3 targetPos = obstacle->GetComponent<Transform>()->GetPosition();
	navigationInstance.RemoveTempObstacle(targetPos);
	//SceneManager::GetInstance().GetCurrentScene()->DeleteGameObject(obstacle);
}

void KunrealEngine::EngineCore::ParticleTest()
{
	// Camera
	DirectX::XMFLOAT3 cameraPos = { 0.0f, 30.0f, -55.0f };
	// KunrealEngine::KunrealMath::Float3 cameraPos = { 40.0f, 2.0f, -30.0f };
	DirectX::XMFLOAT3 targetPos = { 0.0f, -15.0f, 0.0f };
	testCamera = sceneInstance.GetCurrentScene()->CreateObject("testCamera");
	testCamera->AddComponent<Camera>();
	testCamera->GetComponent<Camera>()->SetCameraPosition(cameraPos.x, cameraPos.y, cameraPos.z);
	testCamera->GetComponent<Camera>()->SetTargetPosition(targetPos.x, targetPos.y, targetPos.z);
	//testCamera->GetComponent<Camera>()->RotateCamera(5.f, 0.f);
	testCamera->GetComponent<Camera>()->SetMainCamera();

	//testCamera->GetComponent<Transform>()->SetPosition(-32.f, 45.f, -32.f);
	testCamera->GetComponent<Transform>()->SetPosition(-98.f, 26.f, -90.f);
	testCamera->GetComponent<Transform>()->SetRotation(0.f, 45.f, 60.f);

	// Camera
	DirectX::XMFLOAT3 pcameraPos = { 0.0f, 120.0f, 0.0f };

	particleCamera = sceneInstance.GetCurrentScene()->CreateObject("particleCamera");
	particleCamera->AddComponent<Camera>();
	particleCamera->GetComponent<Camera>()->SetCameraPosition(pcameraPos.x, pcameraPos.y, pcameraPos.z);
	particleCamera->GetComponent<Camera>()->SetTargetPosition(targetPos.x, targetPos.y, -1.0f);

	bossMap = sceneInstance.GetCurrentScene()->CreateObject("bossMap");
	bossMap->AddComponent<MeshRenderer>();
	bossMap->GetComponent<MeshRenderer>()->SetMeshObject("MapMesh/MapMesh");
	bossMap->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	bossMap->GetComponent<Transform>()->SetPosition(0.0f, -10.0f, 0.0f);

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

	//// Player
	//player = sceneInstance.GetCurrentScene()->CreateObject("Player");
	//player->AddComponent<Player>();

	//// Kamen
	//kamen = sceneInstance.GetCurrentScene()->CreateObject("kamen");
	//kamen->AddComponent<Kamen>();

	particle = sceneInstance.GetCurrentScene()->CreateObject("Particle");
	particle->GetComponent<Transform>()->SetPosition(-20, 0, 0);
	particle->AddComponent<Particle>();
	particle->GetComponent<Particle>()->SetParticleEffect("Halo1", "Resources/Textures/Particles/fx_Halo1.dds", 1000);
	particle->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	particle->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	particle->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 200.0f);

	particle2 = sceneInstance.GetCurrentScene()->CreateObject("Particle2");
	particle2->GetComponent<Transform>()->SetPosition(-10, 0, 0);
	particle2->AddComponent<Particle>();
	particle2->GetComponent<Particle>()->SetParticleEffect("fx_Lightning4", "Resources/Textures/Particles/fx_Lightning4.dds", 1000);
	particle2->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle2->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	particle2->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle2->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	particle2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 200.0f);

	particle3 = sceneInstance.GetCurrentScene()->CreateObject("Particle3");
	particle3->GetComponent<Transform>()->SetPosition(0, 0, 0);
	particle3->AddComponent<Particle>();
	particle3->GetComponent<Particle>()->SetParticleEffect("Beam1", "Resources/Textures/Particles/fx_Beam1.dds", 1000);
	particle3->GetComponent<Particle>()->SetParticleDuration(10.0f, 1.0f);
	particle3->GetComponent<Particle>()->SetParticleVelocity(15.0f, true);
	particle3->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle3->GetComponent<Particle>()->AddParticleColor(0.1f, 0.0f, 0.6f);
	particle3->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 30.0f);

	particle4 = sceneInstance.GetCurrentScene()->CreateObject("Particle4");
	particle4->GetComponent<Transform>()->SetPosition(10, 0, 0);
	particle4->AddComponent<Particle>();
	particle4->GetComponent<Particle>()->SetParticleEffect("Beam2", "Resources/Textures/Particles/fx_Beam2.dds", 1000);
	particle4->GetComponent<Particle>()->SetParticleDuration(10.0f, 1.0f);
	particle4->GetComponent<Particle>()->SetParticleVelocity(15.0f, true);
	particle4->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle4->GetComponent<Particle>()->AddParticleColor(0.1f, 0.0f, 0.6f);
	particle4->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 30.0f);

	particle5 = sceneInstance.GetCurrentScene()->CreateObject("Particle5");
	particle5->GetComponent<Transform>()->SetPosition(20, 0, 0);
	particle5->AddComponent<Particle>();
	particle5->GetComponent<Particle>()->SetParticleEffect("LightFlash1", "Resources/Textures/Particles/fx_LightFlash1.dds", 1000);
	particle5->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle5->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	particle5->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle5->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	particle5->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 200.0f);

	particle6 = sceneInstance.GetCurrentScene()->CreateObject("Particle6");
	particle6->GetComponent<Transform>()->SetPosition(30, 0, 0);
	particle6->AddComponent<Particle>();
	particle6->GetComponent<Particle>()->SetParticleEffect("LightFlash2", "Resources/Textures/Particles/fx_LightFlash2.dds", 1000);
	particle6->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle6->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	particle6->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle6->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	particle6->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 200.0f);

	particle7 = sceneInstance.GetCurrentScene()->CreateObject("Particle7");
	particle7->GetComponent<Transform>()->SetPosition(40, 0, 0);
	particle7->AddComponent<Particle>();
	particle7->GetComponent<Particle>()->SetParticleEffect("Lightning1", "Resources/Textures/Particles/fx_Lightning1.dds", 1000);
	particle7->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle7->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	particle7->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle7->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	particle7->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 200.0f);

	particle8 = sceneInstance.GetCurrentScene()->CreateObject("Particle8");
	particle8->GetComponent<Transform>()->SetPosition(50, 0, 0);
	particle8->AddComponent<Particle>();
	particle8->GetComponent<Particle>()->SetParticleEffect("Lightning2", "Resources/Textures/Particles/fx_Lightning2.dds", 1000);
	particle8->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle8->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	particle8->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle8->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	particle8->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 200.0f);

	particle9 = sceneInstance.GetCurrentScene()->CreateObject("Particle9");
	particle9->GetComponent<Transform>()->SetPosition(0, 0, -25.f);
	particle9->AddComponent<Particle>();
	particle9->GetComponent<Particle>()->SetParticleEffect("fx_Dust3", "Resources/Textures/Particles/fx_Dust3.dds", 1000);
	particle9->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle9->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	particle9->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle9->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	particle9->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 200.0f);

	particle10 = sceneInstance.GetCurrentScene()->CreateObject("Particle10");
	particle10->GetComponent<Transform>()->SetPosition(10, 0, -25.f);
	particle10->AddComponent<Particle>();
	particle10->GetComponent<Particle>()->SetParticleEffect("SmokeyHalo1", "Resources/Textures/Particles/fx_SmokeyHalo1.dds", 10);
	particle10->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle10->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	particle10->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle10->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	particle10->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 200.0f);

	particle11 = sceneInstance.GetCurrentScene()->CreateObject("Particle11");
	particle11->GetComponent<Transform>()->SetPosition(-50, 0, 0.f);
	particle11->AddComponent<Particle>();
	particle11->GetComponent<Particle>()->SetParticleEffect("Sparks1", "Resources/Textures/Particles/fx_Sparks1.dds", 1000);
	particle11->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	particle11->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particle11->GetComponent<Particle>()->SetParticleSize(20.f, 20.0f);
	particle11->GetComponent<Particle>()->AddParticleColor(0.0f, 1.0f, 2.0f);
	particle11->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	particle11->GetComponent<Particle>()->SetParticleCameraApply(true);

	particle12 = sceneInstance.GetCurrentScene()->CreateObject("Particle12");
	particle12->GetComponent<Transform>()->SetPosition(30, 0, -25.f);
	particle12->AddComponent<Particle>();
	particle12->GetComponent<Particle>()->SetParticleEffect("Blaster1", "Resources/Textures/Particles/fx_Blaster1.dds", 1000);
	particle12->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle12->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	particle12->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle12->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	particle12->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 200.0f);

	particle13 = sceneInstance.GetCurrentScene()->CreateObject("Particle13");
	particle13->GetComponent<Transform>()->SetPosition(40, 0, -25.f);
	particle13->AddComponent<Particle>();
	particle13->GetComponent<Particle>()->SetParticleEffect("Blaster2", "Resources/Textures/Particles/fx_Blaster2.dds", 1000);
	particle13->GetComponent<Particle>()->SetParticleDuration(0.6f, 0.05f);
	particle13->GetComponent<Particle>()->SetParticleVelocity(45.0f, true);
	particle13->GetComponent<Particle>()->SetParticleSize(15.f, 9.0f);
	particle13->GetComponent<Particle>()->AddParticleColor(0.0f, 1.0f, 4.0f);
	particle13->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particle14 = sceneInstance.GetCurrentScene()->CreateObject("Particle14");
	particle14->GetComponent<Transform>()->SetPosition(-20, 0, 25.f);
	particle14->AddComponent<Particle>();
	particle14->GetComponent<Particle>()->SetParticleEffect("BlastWave1", "Resources/Textures/Particles/fx_BlastWave1.dds", 1000);
	particle14->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle14->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	particle14->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle14->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	particle14->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 200.0f);

	particle15 = sceneInstance.GetCurrentScene()->CreateObject("Particle15");
	particle15->GetComponent<Transform>()->SetPosition(0, 0, 35.f);
	particle15->AddComponent<Particle>();
	particle15->GetComponent<Particle>()->SetParticleEffect("BlastWave5", "Resources/Textures/Particles/fx_BlastWave5.dds", 1000);
	particle15->GetComponent<Particle>()->SetParticleDuration(1.0f, 4.0f);
	particle15->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	particle15->GetComponent<Particle>()->SetParticleSize(7.f, 7.0f);
	particle15->GetComponent<Particle>()->AddParticleColor(6.0f, 0.2f, 0.0f);
	particle15->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 200.0f);

	particle16 = sceneInstance.GetCurrentScene()->CreateObject("Particle16");
	particle16->GetComponent<Transform>()->SetPosition(0, 0, 35.f);
	particle16->AddComponent<Particle>();
	particle16->GetComponent<Particle>()->SetParticleEffect("BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
	particle16->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.0f);
	particle16->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
	particle16->GetComponent<Particle>()->SetParticleSize(10.f, 10.0f);
	particle16->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 2.0f);
	particle16->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	//particle16->GetComponent<Particle>()->SetParticleRotation(90.0f, 0.0f, 0.0f);
	particle16->GetComponent<Particle>()->SetParticleAngle(90.0f);

	particle17 = sceneInstance.GetCurrentScene()->CreateObject("Particle17");
	particle17->GetComponent<Transform>()->SetPosition(10, 0, 35.f);
	particle17->AddComponent<Particle>();
	particle17->GetComponent<Particle>()->SetParticleEffect("Cracks1", "Resources/Textures/Particles/fx_Cracks1.dds", 1000);
	particle17->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.0f);
	particle17->GetComponent<Particle>()->SetParticleVelocity(25.0f, true);
	particle17->GetComponent<Particle>()->SetParticleSize(5.0f, 5.0f);
	particle17->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 2.0f);
	particle17->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 200.0f);

	particle18 = sceneInstance.GetCurrentScene()->CreateObject("Particle18");
	particle18->GetComponent<Transform>()->SetPosition(20, 0, 35.f);
	particle18->AddComponent<Particle>();
	particle18->GetComponent<Particle>()->SetParticleEffect("Dust1", "Resources/Textures/Particles/fx_Dust1.dds", 1000);
	particle18->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.0f);
	particle18->GetComponent<Particle>()->SetParticleVelocity(25.0f, true);
	particle18->GetComponent<Particle>()->SetParticleSize(5.0f, 5.0f);
	particle18->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 2.0f);
	particle18->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 200.0f);

	particle19 = sceneInstance.GetCurrentScene()->CreateObject("Particle19");
	particle19->GetComponent<Transform>()->SetPosition(30, 0, 35.f);
	particle19->AddComponent<Particle>();
	particle19->GetComponent<Particle>()->SetParticleEffect("Dust2", "Resources/Textures/Particles/fx_Dust2.dds", 1000);
	particle19->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.0f);
	particle19->GetComponent<Particle>()->SetParticleVelocity(25.0f, true);
	particle19->GetComponent<Particle>()->SetParticleSize(5.0f, 5.0f);
	particle19->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 2.0f);
	particle19->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 200.0f);

	particle20 = sceneInstance.GetCurrentScene()->CreateObject("Particle20");
	particle20->GetComponent<Transform>()->SetPosition(-10, 0, 70.f);
	particle20->AddComponent<Particle>();
	particle20->GetComponent<Particle>()->SetParticleEffect("fx_BlastWave1", "Resources/Textures/Particles/fx_BlastWave1.dds", 1000);
	particle20->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle20->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particle20->GetComponent<Particle>()->SetParticleSize(5.0f, 5.0f);
	particle20->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 2.0f);
	particle20->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particle21 = sceneInstance.GetCurrentScene()->CreateObject("Particle21");
	particle21->GetComponent<Transform>()->SetPosition(0, 0, 70.f);
	particle21->AddComponent<Particle>();
	particle21->GetComponent<Particle>()->SetParticleEffect("fx_BlastWave2", "Resources/Textures/Particles/fx_BlastWave2.dds", 1000);
	particle21->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle21->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particle21->GetComponent<Particle>()->SetParticleSize(5.0f, 5.0f);
	particle21->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 2.0f);
	particle21->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particle22 = sceneInstance.GetCurrentScene()->CreateObject("Particle22");
	particle22->GetComponent<Transform>()->SetPosition(10, 0, 70.f);
	particle22->AddComponent<Particle>();
	particle22->GetComponent<Particle>()->SetParticleEffect("fx_BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
	particle22->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle22->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particle22->GetComponent<Particle>()->SetParticleSize(5.0f, 5.0f);
	particle22->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 2.0f);
	particle22->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particle23 = sceneInstance.GetCurrentScene()->CreateObject("Particle23");
	particle23->GetComponent<Transform>()->SetPosition(20, 0, 70.f);
	particle23->AddComponent<Particle>();
	particle23->GetComponent<Particle>()->SetParticleEffect("fx_BlastWave4", "Resources/Textures/Particles/fx_BlastWave4.dds", 1000);
	particle23->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle23->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particle23->GetComponent<Particle>()->SetParticleSize(5.0f, 5.0f);
	particle23->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 2.0f);
	particle23->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particle24 = sceneInstance.GetCurrentScene()->CreateObject("Particle24");
	particle24->GetComponent<Transform>()->SetPosition(30, 0, 70.f);
	particle24->AddComponent<Particle>();
	particle24->GetComponent<Particle>()->SetParticleEffect("fx_BlastWave5", "Resources/Textures/Particles/fx_BlastWave5.dds", 1000);
	particle24->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle24->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particle24->GetComponent<Particle>()->SetParticleSize(5.0f, 5.0f);
	particle24->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 2.0f);
	particle24->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particle25 = sceneInstance.GetCurrentScene()->CreateObject("Particle25");
	particle25->GetComponent<Transform>()->SetPosition(-10, 0, 105.f);
	particle25->AddComponent<Particle>();
	particle25->GetComponent<Particle>()->SetParticleEffect("fx_Halo1", "Resources/Textures/Particles/fx_Halo1.dds", 1000);
	particle25->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle25->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particle25->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle25->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	particle25->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particle26 = sceneInstance.GetCurrentScene()->CreateObject("Particle26");
	particle26->GetComponent<Transform>()->SetPosition(0, 0, 105.f);
	particle26->AddComponent<Particle>();
	particle26->GetComponent<Particle>()->SetParticleEffect("fx_Halo1", "Resources/Textures/Particles/fx_Halo2.dds", 1000);
	particle26->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle26->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particle26->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle26->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	particle26->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particle27 = sceneInstance.GetCurrentScene()->CreateObject("Particle27");
	particle27->GetComponent<Transform>()->SetPosition(10, 0, 105.f);
	particle27->AddComponent<Particle>();
	particle27->GetComponent<Particle>()->SetParticleEffect("fx_Halo3", "Resources/Textures/Particles/fx_Halo3.dds", 1000);
	particle27->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle27->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particle27->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle27->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	particle27->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particle28 = sceneInstance.GetCurrentScene()->CreateObject("Particle28");
	particle28->GetComponent<Transform>()->SetPosition(20, 0, 105.f);
	particle28->AddComponent<Particle>();
	particle28->GetComponent<Particle>()->SetParticleEffect("fx_EnergyBolt7", "Resources/Textures/Particles/fx_EnergyBolt7.dds", 1000);
	particle28->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle28->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particle28->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle28->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	particle28->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particle29 = sceneInstance.GetCurrentScene()->CreateObject("Particle29");
	particle29->GetComponent<Transform>()->SetPosition(1.2, 40, 78.5f);
	particle29->AddComponent<Particle>();
	particle29->GetComponent<Particle>()->SetParticleEffect("fx_SmokeyHalo1", "Resources/Textures/Particles/fx_SmokeyHalo1.dds", 1000);
	particle29->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle29->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particle29->GetComponent<Particle>()->SetParticleSize(30.f, 30.0f);
	particle29->GetComponent<Particle>()->AddParticleColor(0.0f, 0.6f, 1.0f);
	particle29->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	particle29->GetComponent<Particle>()->SetParticleCameraApply(true);
	particle29->GetComponent<Particle>()->SetParticleAngle(93.0f, 154.0f, 93.0f);

	particle30 = sceneInstance.GetCurrentScene()->CreateObject("Particle30");
	particle30->GetComponent<Transform>()->SetPosition(-30, 0, -30.f);
	particle30->AddComponent<Particle>();
	particle30->GetComponent<Particle>()->SetParticleEffect("Flare6", "Resources/Textures/Particles/fx_Twister3.dds", 1000);
	particle30->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle30->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particle30->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle30->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	particle30->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);



	// 플레이어 q


	particlePlayerQ1 = sceneInstance.GetCurrentScene()->CreateObject("PlayerQ1");
	particlePlayerQ1->GetComponent<Transform>()->SetPosition(-50, 0, 35.f);
	particlePlayerQ1->AddComponent<Particle>();
	particlePlayerQ1->GetComponent<Particle>()->SetParticleEffect("BlastWave2", "Resources/Textures/Particles/fx_BlastWave2.dds", 1000);
	particlePlayerQ1->GetComponent<Particle>()->SetParticleDuration(0.7f, 4.0f);
	particlePlayerQ1->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	particlePlayerQ1->GetComponent<Particle>()->SetParticleSize(7.f, 7.0f);
	particlePlayerQ1->GetComponent<Particle>()->AddParticleColor(0.1f, 0.1f, 1.0f);
	particlePlayerQ1->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particlePlayerQ2 = sceneInstance.GetCurrentScene()->CreateObject("PlayerQ2");
	particlePlayerQ2->GetComponent<Transform>()->SetPosition(-50, 0, 35.f);
	particlePlayerQ2->AddComponent<Particle>();
	particlePlayerQ2->GetComponent<Particle>()->SetParticleEffect("BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
	particlePlayerQ2->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.0f);
	particlePlayerQ2->GetComponent<Particle>()->SetParticleVelocity(7.0f, true);
	particlePlayerQ2->GetComponent<Particle>()->SetParticleSize(10.f, 10.0f);
	particlePlayerQ2->GetComponent<Particle>()->AddParticleColor(0.0f, 0.3f, 2.0f);
	particlePlayerQ2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particlePlayerQ3 = sceneInstance.GetCurrentScene()->CreateObject("PlayerQ3");
	particlePlayerQ3->GetComponent<Transform>()->SetPosition(-50, 0, 35.f);
	particlePlayerQ3->AddComponent<Particle>();
	particlePlayerQ3->GetComponent<Particle>()->SetParticleEffect("Cracks1", "Resources/Textures/Particles/fx_Cracks1.dds", 1000);
	particlePlayerQ3->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.7f);
	particlePlayerQ3->GetComponent<Particle>()->SetParticleVelocity(25.0f, true);
	particlePlayerQ3->GetComponent<Particle>()->SetParticleSize(2.0f, 2.0f);
	particlePlayerQ3->GetComponent<Particle>()->AddParticleColor(0.0f, 0.3f, 2.0f);
	particlePlayerQ3->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particlePlayerQ4 = sceneInstance.GetCurrentScene()->CreateObject("PlayerQ4");
	particlePlayerQ4->GetComponent<Transform>()->SetPosition(-50, 0, 35.f);
	particlePlayerQ4->AddComponent<Particle>();
	particlePlayerQ4->GetComponent<Particle>()->SetParticleEffect("EnergyBolt1", "Resources/Textures/Particles/fx_EnergyBolt1.dds", 1000);
	particlePlayerQ4->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.0f);
	particlePlayerQ4->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
	particlePlayerQ4->GetComponent<Particle>()->SetParticleSize(1.0f, 1.0f);
	particlePlayerQ4->GetComponent<Particle>()->AddParticleColor(0.0f, 0.3f, 2.0f);
	particlePlayerQ4->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);


	// 플레이어 W, 왜 다시 안나오는가?

	GameObject* ice = sceneInstance.GetCurrentScene()->CreateObject("Ice");
	ice->GetComponent<Transform>()->SetScale(15.0f, 15.0f, 15.0f);
	ice->GetComponent<Transform>()->SetPosition(-50, 0, 0.f);
	ice->GetComponent<Transform>()->SetRotation(90.0f, 0.0f, 0.0f);
	ice->AddComponent<MeshRenderer>()->SetMeshObject("Ice/Ice", "Ice/Ice.png");

	particlePlayerW1 = sceneInstance.GetCurrentScene()->CreateObject("PlayerW1");
	particlePlayerW1->GetComponent<Transform>()->SetPosition(-50, 3, 0.f);
	particlePlayerW1->AddComponent<Particle>();
	particlePlayerW1->GetComponent<Particle>()->SetParticleEffect("Blast3", "Resources/Textures/Particles/fx_Blast3.dds", 100);
	particlePlayerW1->GetComponent<Particle>()->SetParticleDuration(2.0f, 2.0f);
	particlePlayerW1->GetComponent<Particle>()->SetParticleVelocity(4.0f, true);
	particlePlayerW1->GetComponent<Particle>()->SetParticleSize(3.f, 3.0f);
	particlePlayerW1->GetComponent<Particle>()->AddParticleColor(0.5f, 3.0f, 5.0f);
	particlePlayerW1->GetComponent<Particle>()->SetParticleDirection(0.0f, -1.0f, 0.0f);
	particlePlayerW1->GetComponent<Particle>()->SetParticleAngle(339.0f, 0.0f, 30.0f);
	particlePlayerW1->GetComponent<Particle>()->SetParticleCameraApply(true);

	particlePlayerW2 = sceneInstance.GetCurrentScene()->CreateObject("PlayerW2");
	particlePlayerW2->GetComponent<Transform>()->SetPosition(-50, 0, 0.f);
	particlePlayerW2->AddComponent<Particle>();
	particlePlayerW2->GetComponent<Particle>()->SetParticleEffect("Dust3", "Resources/Textures/Particles/fx_Dust3.dds", 100);
	particlePlayerW2->GetComponent<Particle>()->SetParticleDuration(4.0f, 2.0f);
	particlePlayerW2->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	particlePlayerW2->GetComponent<Particle>()->SetParticleSize(3.0f, 3.0f);
	particlePlayerW2->GetComponent<Particle>()->AddParticleColor(0.5f, 3.0f, 5.0f);
	particlePlayerW2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	particlePlayerW2->GetComponent<Particle>()->SetParticleAngle(339.0f, 0.0f, 30.0f);
	particlePlayerW2->GetComponent<Particle>()->SetParticleCameraApply(true);

	//플레이어 R

	GameObject* meteor = sceneInstance.GetCurrentScene()->CreateObject("Meteor");;
	meteor->GetComponent<Transform>()->SetPosition(-47.3f, 11, 60);
	meteor->GetComponent<Transform>()->SetRotation(0, 0, 90);
	meteor->AddComponent<MeshRenderer>();
	meteor->GetComponent<MeshRenderer>()->SetMeshObject("Meteor/Meteor");

	particlePlayerR1 = sceneInstance.GetCurrentScene()->CreateObject("PlayerR1");
	particlePlayerR1->GetComponent<Transform>()->SetPosition(-47.5f, 10, 60.f);
	particlePlayerR1->AddComponent<Particle>();
	particlePlayerR1->GetComponent<Particle>()->SetParticleEffect("BlastWave2", "Resources/Textures/Particles/fx_BlastWave2.dds", 1000);
	particlePlayerR1->GetComponent<Particle>()->SetParticleDuration(1.5f, 1.5f);
	particlePlayerR1->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	particlePlayerR1->GetComponent<Particle>()->SetParticleSize(17.f, 20.0f);
	particlePlayerR1->GetComponent<Particle>()->AddParticleColor(6.0f, 0.2f, 0.1f);
	particlePlayerR1->GetComponent<Particle>()->SetParticleDirection(0.0f, 50.0f, 0.0f);

	particlePlayerR2 = sceneInstance.GetCurrentScene()->CreateObject("PlayerR2");
	particlePlayerR2->GetComponent<Transform>()->SetPosition(-47.6f, 10, 59.5f);
	particlePlayerR2->AddComponent<Particle>();
	particlePlayerR2->GetComponent<Particle>()->SetParticleEffect("BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
	particlePlayerR2->GetComponent<Particle>()->SetParticleDuration(2.0f, 1.4f);
	particlePlayerR2->GetComponent<Particle>()->SetParticleVelocity(6.5f, true);
	particlePlayerR2->GetComponent<Particle>()->SetParticleSize(17.f, 20.0f);
	particlePlayerR2->GetComponent<Particle>()->AddParticleColor(2.0f, 1.0f, 0.0f);
	particlePlayerR2->GetComponent<Particle>()->SetParticleDirection(0.0f, 50.0f, 0.0f);

	particlePlayerR3 = sceneInstance.GetCurrentScene()->CreateObject("PlayerR3");
	particlePlayerR3->GetComponent<Transform>()->SetPosition(-47.5f, 21.7f, 60.6f);
	particlePlayerR3->AddComponent<Particle>();
	particlePlayerR3->GetComponent<Particle>()->SetParticleEffect("Fire1", "Resources/Textures/Particles/fx_Fire1.dds", 1000);
	particlePlayerR3->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.6f);
	particlePlayerR3->GetComponent<Particle>()->SetParticleVelocity(6.0f, true);
	particlePlayerR3->GetComponent<Particle>()->SetParticleSize(13.f, 13.0f);
	particlePlayerR3->GetComponent<Particle>()->AddParticleColor(1.0f, 0.1f, 0.1f);
	particlePlayerR3->GetComponent<Particle>()->SetParticleDirection(0.0f, 100.0f, 0.0f);
	particlePlayerR3->GetComponent<Particle>()->SetParticleRotation(0.0f, 0.0f, 90.0f);

	particlePlayerR4 = sceneInstance.GetCurrentScene()->CreateObject("PlayerR4");
	particlePlayerR4->GetComponent<Transform>()->SetPosition(-49.3f, 21.7f, 55.3f);
	particlePlayerR4->AddComponent<Particle>();
	particlePlayerR4->GetComponent<Particle>()->SetParticleEffect("Fire1", "Resources/Textures/Particles/fx_Fire1.dds", 1000);
	particlePlayerR4->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.6f);
	particlePlayerR4->GetComponent<Particle>()->SetParticleVelocity(6.0f, true);
	particlePlayerR4->GetComponent<Particle>()->SetParticleSize(10.f, 10.0f);
	particlePlayerR4->GetComponent<Particle>()->AddParticleColor(1.0f, 0.1f, 0.1f);
	particlePlayerR4->GetComponent<Particle>()->SetParticleDirection(0.0f, 100.0f, 0.0f);

	particlePlayerR5 = sceneInstance.GetCurrentScene()->CreateObject("PlayerR5");
	particlePlayerR5->GetComponent<Transform>()->SetPosition(-23, 13, 56.5f);
	particlePlayerR5->AddComponent<Particle>();
	particlePlayerR5->GetComponent<Particle>()->SetParticleEffect("Fire1", "Resources/Textures/Particles/fx_Fire1.dds", 1000);
	particlePlayerR5->GetComponent<Particle>()->SetParticleDuration(2.0f, 0.6f);
	particlePlayerR5->GetComponent<Particle>()->SetParticleVelocity(6.0f, true);
	particlePlayerR5->GetComponent<Particle>()->SetParticleSize(3.f, 3.0f);
	particlePlayerR5->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	particlePlayerR5->GetComponent<Particle>()->SetParticleDirection(0.0f, 100.0f, 0.0f);


	// 포탈 임시

	particlePortal1 = sceneInstance.GetCurrentScene()->CreateObject("particlePortal1");
	particlePortal1->GetComponent<Transform>()->SetPosition(70, 0, 50);
	particlePortal1->AddComponent<Particle>();
	particlePortal1->GetComponent<Particle>()->SetParticleEffect("LightFlash2", "Resources/Textures/Particles/fx_LightFlash2.dds", 1000);
	particlePortal1->GetComponent<Particle>()->SetParticleDuration(0.4f, 0.5f);
	particlePortal1->GetComponent<Particle>()->SetParticleVelocity(32.0f, true);
	particlePortal1->GetComponent<Particle>()->SetParticleSize(10.7f, 20.0f);
	particlePortal1->GetComponent<Particle>()->AddParticleColor(0.0f, 0.1f, 0.5f);
	particlePortal1->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particlePortal2 = sceneInstance.GetCurrentScene()->CreateObject("particlePortal2");
	particlePortal2->GetComponent<Transform>()->SetPosition(70, 0, 50);
	particlePortal2->AddComponent<Particle>();
	particlePortal2->GetComponent<Particle>()->SetParticleEffect("BlastWave1", "Resources/Textures/Particles/fx_BlastWave1.dds", 1000);
	particlePortal2->GetComponent<Particle>()->SetParticleDuration(0.1f, 0.1f);
	particlePortal2->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
	particlePortal2->GetComponent<Particle>()->SetParticleSize(5.f, 10.3f);
	particlePortal2->GetComponent<Particle>()->AddParticleColor(0.0f, 0.0f, 0.6f);
	particlePortal2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 200.0f);

	particlePortal3 = sceneInstance.GetCurrentScene()->CreateObject("particlePortal3");
	particlePortal3->GetComponent<Transform>()->SetPosition(70, 0, 50);
	particlePortal3->AddComponent<Particle>();
	particlePortal3->GetComponent<Particle>()->SetParticleEffect("BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
	particlePortal3->GetComponent<Particle>()->SetParticleDuration(3.1f, 0.4f);
	particlePortal3->GetComponent<Particle>()->SetParticleVelocity(3.6f, true);
	particlePortal3->GetComponent<Particle>()->SetParticleSize(5.f, 12.5f);
	particlePortal3->GetComponent<Particle>()->AddParticleColor(0.0f, 0.2f, 1.6f);
	particlePortal3->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	// 보스 콜 패턴
	particleBossCall1 = sceneInstance.GetCurrentScene()->CreateObject("particleBossCall1");
	particleBossCall1->GetComponent<Transform>()->SetPosition(-70, 0, 70);
	particleBossCall1->AddComponent<Particle>();
	particleBossCall1->GetComponent<Particle>()->SetParticleEffect("fx_Halo3", "Resources/Textures/Particles/fx_Halo3.dds", 1000);
	particleBossCall1->GetComponent<Particle>()->SetParticleDuration(1.2f, 2.0f);
	particleBossCall1->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particleBossCall1->GetComponent<Particle>()->SetParticleSize(9.f, 3.0f);
	particleBossCall1->GetComponent<Particle>()->AddParticleColor(1.0f, 5.f, 0.0f);
	particleBossCall1->GetComponent<Particle>()->SetParticleDirection(0.0f, 50.0f, 0.0f);

	particleBossCall2 = sceneInstance.GetCurrentScene()->CreateObject("particleBossCall2");
	particleBossCall2->GetComponent<Transform>()->SetPosition(-70, 0.0f, 70.f);
	particleBossCall2->AddComponent<Particle>();
	particleBossCall2->GetComponent<Particle>()->SetParticleEffect("fx_Sparks1", "Resources/Textures/Particles/fx_Sparks1.dds", 1000);
	particleBossCall2->GetComponent<Particle>()->SetParticleDuration(1.3f, 1.0f);
	particleBossCall2->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	particleBossCall2->GetComponent<Particle>()->SetParticleSize(8.f, 8.f);
	particleBossCall2->GetComponent<Particle>()->AddParticleColor(0.0f, 5.0f, 0.0f);
	particleBossCall2->GetComponent<Particle>()->SetParticleDirection(0.0f, 50.0f, 0.0f);

	particleBossCall3 = sceneInstance.GetCurrentScene()->CreateObject("particleBossCall3");
	particleBossCall3->GetComponent<Transform>()->SetPosition(-70, 0, 70.f);
	particleBossCall3->AddComponent<Particle>();
	particleBossCall3->GetComponent<Particle>()->SetParticleEffect("fx_Sparks1", "Resources/Textures/Particles/fx_Sparks1.dds", 1000);
	particleBossCall3->GetComponent<Particle>()->SetParticleDuration(2.0f, 1.0f);
	particleBossCall3->GetComponent<Particle>()->SetParticleVelocity(3.f, true);
	particleBossCall3->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particleBossCall3->GetComponent<Particle>()->AddParticleColor(1.0f, 1.0f, 1.0f);
	particleBossCall3->GetComponent<Particle>()->SetParticleDirection(0.0f, 50.0f, 0.0f);


	// 보스 거대 기둥출력

	particleBossPillar1 = sceneInstance.GetCurrentScene()->CreateObject("particleBossPillar1");
	particleBossPillar1->GetComponent<Transform>()->SetPosition(35, 7.5f, -70);
	particleBossPillar1->AddComponent<Particle>();
	particleBossPillar1->GetComponent<Particle>()->SetParticleEffect("Halo1", "Resources/Textures/Particles/fx_Halo1.dds", 1000);
	particleBossPillar1->GetComponent<Particle>()->SetParticleDuration(2.1f, 4.f);
	particleBossPillar1->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	particleBossPillar1->GetComponent<Particle>()->SetParticleSize(10.f, 10.0f);
	particleBossPillar1->GetComponent<Particle>()->AddParticleColor(0.0f, 0.6f, 0.0f);
	particleBossPillar1->GetComponent<Particle>()->SetParticleDirection(0.0f, 80.0f, 0.0f);

	particleBossPillar2 = sceneInstance.GetCurrentScene()->CreateObject("particleBossPillar2");
	particleBossPillar2->GetComponent<Transform>()->SetPosition(35, 0, -70);
	particleBossPillar2->AddComponent<Particle>();
	particleBossPillar2->GetComponent<Particle>()->SetParticleEffect("Sparks1", "Resources/Textures/Particles/fx_Sparks1.dds", 1000);
	particleBossPillar2->GetComponent<Particle>()->SetParticleDuration(6.4f, 1.2f);
	particleBossPillar2->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particleBossPillar2->GetComponent<Particle>()->SetParticleSize(20.f, 20.0f);
	particleBossPillar2->GetComponent<Particle>()->AddParticleColor(0.6f, 1.5f, 0.4f);
	particleBossPillar2->GetComponent<Particle>()->SetParticleDirection(0.0f, 80.0f, 0.0f);

	particleBossPillar3 = sceneInstance.GetCurrentScene()->CreateObject("particleBossPillar3");
	particleBossPillar3->GetComponent<Transform>()->SetPosition(35, 11.5f, -70);
	particleBossPillar3->AddComponent<Particle>();
	particleBossPillar3->GetComponent<Particle>()->SetParticleEffect("Blaster2", "Resources/Textures/Particles/fx_Blaster2.dds", 1000);
	particleBossPillar3->GetComponent<Particle>()->SetParticleDuration(4.1f, 1.15f);
	particleBossPillar3->GetComponent<Particle>()->SetParticleVelocity(0.4f, true);
	particleBossPillar3->GetComponent<Particle>()->SetParticleSize(13.f, 10.0f);
	particleBossPillar3->GetComponent<Particle>()->AddParticleColor(0.0f, 0.0f, 2.0f);
	particleBossPillar3->GetComponent<Particle>()->SetParticleDirection(0.0f, 80.0f, 0.0f);

	// 보스 평타 불덩이

	particleBossShot1 = sceneInstance.GetCurrentScene()->CreateObject("particleBossShot1");
	particleBossShot1->GetComponent<Transform>()->SetPosition(-84.3, 0, 35.f);
	particleBossShot1->AddComponent<Particle>();
	particleBossShot1->GetComponent<Particle>()->SetParticleEffect("BlastWave2", "Resources/Textures/Particles/fx_BlastWave2.dds", 1000);
	particleBossShot1->GetComponent<Particle>()->SetParticleDuration(1.0f, 4.0f);
	particleBossShot1->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
	particleBossShot1->GetComponent<Particle>()->SetParticleSize(7.f, 7.0f);
	particleBossShot1->GetComponent<Particle>()->AddParticleColor(1.2f, 7.5f, 0.6f);
	particleBossShot1->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 200.0f);

	particleBossShot2 = sceneInstance.GetCurrentScene()->CreateObject("particleBossShot2");
	particleBossShot2->GetComponent<Transform>()->SetPosition(-84.3, 0, 35.f);
	particleBossShot2->AddComponent<Particle>();
	particleBossShot2->GetComponent<Particle>()->SetParticleEffect("BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
	particleBossShot2->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.7f);
	particleBossShot2->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	particleBossShot2->GetComponent<Particle>()->SetParticleSize(10.f, 10.0f);
	particleBossShot2->GetComponent<Particle>()->AddParticleColor(1.5f, 7.5f, 0.4f);
	particleBossShot2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 200.0f);

	// 카멘 소드

	GameObject* sword = sceneInstance.GetCurrentScene()->CreateObject("sword");
	sword->AddComponent<MeshRenderer>();
	sword->GetComponent<MeshRenderer>()->SetMeshObject("KamenSword/KamenSword");
	sword->GetComponent<MeshRenderer>()->SetDiffuseTexture(0, "KamenSword/KamenSword_BaseColor.png");
	sword->GetComponent<MeshRenderer>()->SetNormalTexture(0, "KamenSword/KamenSword_Normal.png");
	sword->GetComponent<MeshRenderer>()->SetEmissiveTexture(0, "KamenSword/KamenSword_Emissive.png");

	sword->GetComponent<Transform>()->SetPosition(-84, 14, 0);
	sword->GetComponent<Transform>()->SetRotation(180, 90, 0);

	particleBossSword1 = sceneInstance.GetCurrentScene()->CreateObject("particleBossSword1");
	particleBossSword1->GetComponent<Transform>()->SetPosition(-82.6, 6.65, -31.9f);
	particleBossSword1->AddComponent<Particle>();
	particleBossSword1->GetComponent<Particle>()->SetParticleEffect("BlastWave1", "Resources/Textures/Particles/fx_BlastWave1.dds", 1000);
	particleBossSword1->GetComponent<Particle>()->SetParticleDuration(1.1f, 2.3f);
	particleBossSword1->GetComponent<Particle>()->SetParticleVelocity(0.6f, true);
	particleBossSword1->GetComponent<Particle>()->SetParticleSize(7.f, 7.0f);
	particleBossSword1->GetComponent<Particle>()->AddParticleColor(1.2f, 7.5f, 0.0f);
	particleBossSword1->GetComponent<Particle>()->SetParticleDirection(0.0f, 50.0f, 0.0f);

	particleBossSword2 = sceneInstance.GetCurrentScene()->CreateObject("particleBossSword2");
	particleBossSword2->GetComponent<Transform>()->SetPosition(-84.3, 10.8, -33.7f);
	particleBossSword2->AddComponent<Particle>();
	particleBossSword2->GetComponent<Particle>()->SetParticleEffect("BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
	particleBossSword2->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.0f);
	particleBossSword2->GetComponent<Particle>()->SetParticleVelocity(0.2f, true);
	particleBossSword2->GetComponent<Particle>()->SetParticleSize(8.4f, 7.0f);
	particleBossSword2->GetComponent<Particle>()->AddParticleColor(1.0f, 7.5f, 0.0f);
	particleBossSword2->GetComponent<Particle>()->SetParticleDirection(0.0f, 50.0f, 0.0f);

	particleBossSword3 = sceneInstance.GetCurrentScene()->CreateObject("particleBossSword3");
	particleBossSword3->GetComponent<Transform>()->SetPosition(-83.4f, 21.3f, -32.8f);
	particleBossSword3->AddComponent<Particle>();
	particleBossSword3->GetComponent<Particle>()->SetParticleEffect("Cracks1", "Resources/Textures/Particles/fx_Cracks1.dds", 1000);
	particleBossSword3->GetComponent<Particle>()->SetParticleDuration(2.f, 0.6f);
	particleBossSword3->GetComponent<Particle>()->SetParticleVelocity(9.6f, true);
	particleBossSword3->GetComponent<Particle>()->SetParticleSize(2.0f, 2.0f);
	particleBossSword3->GetComponent<Particle>()->AddParticleColor(1.2f, 7.5f, 0);
	particleBossSword3->GetComponent<Particle>()->SetParticleDirection(0.0f, 10.0f, 0.0f);

	particleBossSword4 = sceneInstance.GetCurrentScene()->CreateObject("particleBossSword4");
	particleBossSword4->GetComponent<Transform>()->SetPosition(-82.7, 13.8, -32.2f);
	particleBossSword4->AddComponent<Particle>();
	particleBossSword4->GetComponent<Particle>()->SetParticleEffect("Cracks1", "Resources/Textures/Particles/fx_Cracks1.dds", 1000);
	particleBossSword4->GetComponent<Particle>()->SetParticleDuration(2.0f, 0.6f);
	particleBossSword4->GetComponent<Particle>()->SetParticleVelocity(12.2f, true);
	particleBossSword4->GetComponent<Particle>()->SetParticleSize(2.0f, 2.0f);
	particleBossSword4->GetComponent<Particle>()->AddParticleColor(1.2f, 7.5f, 0.f);
	particleBossSword4->GetComponent<Particle>()->SetParticleDirection(0.0f, 10.0f, 0.0f);

	GameObject* sword2 = sceneInstance.GetCurrentScene()->CreateObject("sword2");
	sword2->AddComponent<MeshRenderer>();
	sword2->GetComponent<MeshRenderer>()->SetMeshObject("KamenSword/KamenSword");
	sword2->GetComponent<MeshRenderer>()->SetDiffuseTexture(0, "KamenSword/KamenSword_BaseColor.png");
	sword2->GetComponent<MeshRenderer>()->SetNormalTexture(0, "KamenSword/KamenSword_Normal.png");
	sword2->GetComponent<MeshRenderer>()->SetEmissiveTexture(0, "KamenSword/KamenSword_Emissive.png");

	sword2->GetComponent<Transform>()->SetPosition(-84.3, 0, -55.f);
	sword2->GetComponent<Transform>()->SetRotation(0, 135, 0);

	particleBossSword2_1 = sceneInstance.GetCurrentScene()->CreateObject("particleBossSword2_1");
	particleBossSword2_1->GetComponent<Transform>()->SetPosition(-85.9, 4.8, -54.4);
	particleBossSword2_1->AddComponent<Particle>();
	particleBossSword2_1->GetComponent<Particle>()->SetParticleEffect("Lightning1", "Resources/Textures/Particles/fx_Lightning1.dds", 1000);
	particleBossSword2_1->GetComponent<Particle>()->SetParticleDuration(0.3f, 0.7f);
	particleBossSword2_1->GetComponent<Particle>()->SetParticleVelocity(4.0f, true);
	particleBossSword2_1->GetComponent<Particle>()->SetParticleSize(3.f, 3.0f);
	particleBossSword2_1->GetComponent<Particle>()->AddParticleColor(0.1f, 0.2f, 0.0f);
	particleBossSword2_1->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particleBossSword2_2 = sceneInstance.GetCurrentScene()->CreateObject("particleBossSword2_2");
	particleBossSword2_2->GetComponent<Transform>()->SetPosition(-85.1, 4.8, -55.2);
	particleBossSword2_2->AddComponent<Particle>();
	particleBossSword2_2->GetComponent<Particle>()->SetParticleEffect("Lightning1", "Resources/Textures/Particles/fx_Lightning1.dds", 1000);
	particleBossSword2_2->GetComponent<Particle>()->SetParticleDuration(0.3f, 0.7f);
	particleBossSword2_2->GetComponent<Particle>()->SetParticleVelocity(4.0f, true);
	particleBossSword2_2->GetComponent<Particle>()->SetParticleSize(3.f, 3.0f);
	particleBossSword2_2->GetComponent<Particle>()->AddParticleColor(0.1f, 0.2f, 0.0f);
	particleBossSword2_2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particleBossSword2_3 = sceneInstance.GetCurrentScene()->CreateObject("particleBossSword2_3");
	particleBossSword2_3->GetComponent<Transform>()->SetPosition(-82.5, 8.6, -52);
	particleBossSword2_3->AddComponent<Particle>();
	particleBossSword2_3->GetComponent<Particle>()->SetParticleEffect("EnergyBolt9", "Resources/Textures/Particl   es/fx_EnergyBolt9.dds", 1000);
	particleBossSword2_3->GetComponent<Particle>()->SetParticleDuration(1.2f, 2.0f);
	particleBossSword2_3->GetComponent<Particle>()->SetParticleVelocity(0.1f, true);
	particleBossSword2_3->GetComponent<Particle>()->SetParticleSize(17.0f, 15.0f);
	particleBossSword2_3->GetComponent<Particle>()->AddParticleColor(0.1f, 0.5f, 0.0f);
	particleBossSword2_3->GetComponent<Particle>()->SetParticleDirection(0.0f, 50.0f, 0.0f);

	particleBossSword2_4 = sceneInstance.GetCurrentScene()->CreateObject("particleBossSword2_4");
	particleBossSword2_4->GetComponent<Transform>()->SetPosition(-85.8f, 4.8f, -74.1f);
	particleBossSword2_4->AddComponent<Particle>();
	particleBossSword2_4->GetComponent<Particle>()->SetParticleEffect("LightFlash2", "Resources/Textures/Particles/fx_LightFlash2.dds", 1000);
	particleBossSword2_4->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particleBossSword2_4->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particleBossSword2_4->GetComponent<Particle>()->SetParticleSize(2.f, 2.0f);
	particleBossSword2_4->GetComponent<Particle>()->AddParticleColor(1.0f, 5.0f, 1.0f);
	particleBossSword2_4->GetComponent<Particle>()->SetParticleDirection(0.0f, -15.0f, 30.0f);

	GameObject* sword3 = sceneInstance.GetCurrentScene()->CreateObject("sword");
	sword3->AddComponent<MeshRenderer>();
	sword3->GetComponent<MeshRenderer>()->SetMeshObject("KamenSword/KamenSword");
	sword3->GetComponent<MeshRenderer>()->SetDiffuseTexture(0, "KamenSword/KamenSword_BaseColor.png");
	sword3->GetComponent<MeshRenderer>()->SetNormalTexture(0, "KamenSword/KamenSword_Normal.png");
	sword3->GetComponent<MeshRenderer>()->SetEmissiveTexture(0, "KamenSword/KamenSword_Emissive.png");

	sword3->GetComponent<Transform>()->SetPosition(-84.3, 0, -75.f);
	sword3->GetComponent<Transform>()->SetRotation(0, 135, 0);


	//GameObject* testPlayer = sceneInstance.GetCurrentScene()->CreateObject("Player");
	//testPlayer->AddComponent<Player>();
	//
	//GameObject* testKamen = sceneInstance.GetCurrentScene()->CreateObject("Kamen");
	//testKamen->AddComponent<Kamen>();

	// 도넛 패턴에 배치할 파이클 테스트

	particleBossDonut1 = sceneInstance.GetCurrentScene()->CreateObject("particleBossDonut1");
	particleBossDonut1->GetComponent<Transform>()->SetPosition(-48, -4.4, -65.f);
	particleBossDonut1->AddComponent<Particle>();
	particleBossDonut1->GetComponent<Particle>()->SetParticleEffect("BlastWave5", "Resources/Textures/Particles/fx_BlastWave5.dds", 1000);
	particleBossDonut1->GetComponent<Particle>()->SetParticleDuration(2.0f, 0.2f);
	particleBossDonut1->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
	particleBossDonut1->GetComponent<Particle>()->SetParticleSize(45.f, 45.0f);
	particleBossDonut1->GetComponent<Particle>()->AddParticleColor(1.0f, 1.0f, 0.3f);
	particleBossDonut1->GetComponent<Particle>()->SetParticleDirection(0.0f, 600.0f, 0.0f);
	particleBossDonut1->GetComponent<Particle>()->SetParticleCameraApply(true);

	particleBossDonut2 = sceneInstance.GetCurrentScene()->CreateObject("particleBossDonut2");
	particleBossDonut2->GetComponent<Transform>()->SetPosition(-48, -4.4, -65.f);
	particleBossDonut2->AddComponent<Particle>();
	particleBossDonut2->GetComponent<Particle>()->SetParticleEffect("BlastWave2", "Resources/Textures/Particles/fx_BlastWave2.dds", 1000);
	particleBossDonut2->GetComponent<Particle>()->SetParticleDuration(2.0f, 2.0f);
	particleBossDonut2->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
	particleBossDonut2->GetComponent<Particle>()->SetParticleSize(75.f, 75.0f);
	particleBossDonut2->GetComponent<Particle>()->AddParticleColor(0.05f, 0.1f, 0.0f);
	particleBossDonut2->GetComponent<Particle>()->SetParticleDirection(0.0f, 600.0f, 0.0f);
	particleBossDonut2->GetComponent<Particle>()->SetParticleCameraApply(true);

	particleBossDonut3 = sceneInstance.GetCurrentScene()->CreateObject("particleBossDonut3");
	particleBossDonut3->GetComponent<Transform>()->SetPosition(-48, -4.4, -65.f);
	particleBossDonut3->AddComponent<Particle>();
	particleBossDonut3->GetComponent<Particle>()->SetParticleEffect("fx_Halo1", "Resources/Textures/Particles/fx_Halo1.dds", 1000);
	particleBossDonut3->GetComponent<Particle>()->SetParticleDuration(2.0f, 0.2f);
	particleBossDonut3->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
	particleBossDonut3->GetComponent<Particle>()->SetParticleSize(60.f, 60.0f);
	particleBossDonut3->GetComponent<Particle>()->AddParticleColor(0.0f, 1.0f, 0.3f);
	particleBossDonut3->GetComponent<Particle>()->SetParticleDirection(0.0f, 600.0f, 0.0f);
	particleBossDonut3->GetComponent<Particle>()->SetParticleCameraApply(true);

	particleBossDonut4 = sceneInstance.GetCurrentScene()->CreateObject("particleBossDonut4");
	particleBossDonut4->GetComponent<Transform>()->SetPosition(-48, -4.4, -65.f);
	particleBossDonut4->AddComponent<Particle>();
	particleBossDonut4->GetComponent<Particle>()->SetParticleEffect("fx_Halo2", "Resources/Textures/Particles/fx_Halo2.dds", 1000);
	particleBossDonut4->GetComponent<Particle>()->SetParticleDuration(2.0f, 2.0f);
	particleBossDonut4->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
	particleBossDonut4->GetComponent<Particle>()->SetParticleSize(30.f, 30.0f);
	particleBossDonut4->GetComponent<Particle>()->AddParticleColor(1.0f, 0.1f, 0.1f);
	particleBossDonut4->GetComponent<Particle>()->SetParticleDirection(0.0f, 400.0f, 0.0f);
	particleBossDonut4->GetComponent<Particle>()->SetParticleCameraApply(true);

	/*for (int i = 0; i < 16; ++i)
	{
		GameObject* particleBezierTest;
		particleBezierTest = sceneInstance.GetCurrentScene()->CreateObject("Particle18");
		particleBezierTest->GetComponent<Transform>()->SetPosition(ToolBox::GetRandomFloat(-50.0f, 50.0f), ToolBox::GetRandomFloat(-20.0f, 20.0f) , ToolBox::GetRandomFloat(-62.0f, 62.0f));
		particleBezierTest->AddComponent<Particle>();
		particleBezierTest->GetComponent<Particle>()->SetParticleEffect("Dust1", "Resources/Textures/Particles/fx_Dust1.dds", 1000);
		particleBezierTest->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.0f);
		particleBezierTest->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
		particleBezierTest->GetComponent<Particle>()->SetParticleSize(5.0f, 5.0f);
		particleBezierTest->GetComponent<Particle>()->AddParticleColor(1.0f, 5.0f, 1.0f);
		particleBezierTest->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

		_bezierObjectList.push_back(particleBezierTest);
	}*/

	for (int i = 0; i < 16; ++i)
	{
		GameObject* particleBezierTest;
		particleBezierTest = sceneInstance.GetCurrentScene()->CreateObject("ParticleSwordSoul");
		particleBezierTest->GetComponent<Transform>()->SetPosition(sword2->GetComponent<Transform>()->GetPosition().x + ToolBox::GetRandomFloat(-50.0f, 50.0f),
			sword2->GetComponent<Transform>()->GetPosition().y + ToolBox::GetRandomFloat(-20.0f, 20.0f), sword2->GetComponent<Transform>()->GetPosition().z + ToolBox::GetRandomFloat(-62.0f, 62.0f));
		particleBezierTest->AddComponent<Particle>();
		particleBezierTest->GetComponent<Particle>()->SetParticleEffect("Dust1", "Resources/Textures/Particles/fx_Dust1.dds", 1000);
		particleBezierTest->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.0f);
		particleBezierTest->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
		particleBezierTest->GetComponent<Particle>()->SetParticleSize(2.0f, 2.0f);
		particleBezierTest->GetComponent<Particle>()->AddParticleColor(1.0f, 5.0f, 0.0f);
		particleBezierTest->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

		ParticleSwordSoulList.push_back(particleBezierTest);
	}

	/*particleBezierTest1 = sceneInstance.GetCurrentScene()->CreateObject("Particle18");
	particleBezierTest1->GetComponent<Transform>()->SetPosition(-10, 0, -62.f);
	particleBezierTest1->AddComponent<Particle>();
	particleBezierTest1->GetComponent<Particle>()->SetParticleEffect("Dust1", "Resources/Textures/Particles/fx_Dust1.dds", 1000);
	particleBezierTest1->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.0f);
	particleBezierTest1->GetComponent<Particle>()->SetParticleVelocity(25.0f, true);
	particleBezierTest1->GetComponent<Particle>()->SetParticleSize(5.0f, 5.0f);
	particleBezierTest1->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 2.0f);
	particleBezierTest1->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	bezierObjectList.push_back(particleBezierTest1);

	particleBezierTest2 = sceneInstance.GetCurrentScene()->CreateObject("Particle18");
	particleBezierTest2->GetComponent<Transform>()->SetPosition(-60, 0, -62.f);
	particleBezierTest2->AddComponent<Particle>();
	particleBezierTest2->GetComponent<Particle>()->SetParticleEffect("Dust1", "Resources/Textures/Particles/fx_Dust1.dds", 1000);
	particleBezierTest2->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.0f);
	particleBezierTest2->GetComponent<Particle>()->SetParticleVelocity(25.0f, true);
	particleBezierTest2->GetComponent<Particle>()->SetParticleSize(5.0f, 5.0f);
	particleBezierTest2->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 2.0f);
	particleBezierTest2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	bezierObjectList.push_back(particleBezierTest2);*/

	playerE1 = sceneInstance.GetCurrentScene()->CreateObject("PlayerE1");
	playerE1->GetComponent<Transform>()->SetPosition(-68, 7, 7.f);
	playerE1->AddComponent<Particle>();
	playerE1->GetComponent<Particle>()->SetParticleEffect("BlastWave1", "Resources/Textures/Particles/fx_BlastWave1.dds", 1000);
	playerE1->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.0f);
	playerE1->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	playerE1->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	playerE1->GetComponent<Particle>()->AddParticleColor(0.7f, 0.7f, 0.0f);
	playerE1->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	playerE1->GetComponent<Particle>()->SetParticleAngle(335, 0, 30);
	playerE1->GetComponent<Particle>()->SetParticleCameraApply(true);
	//playerE1->GetComponent<Particle>()->SetParticleAngle(60.f);

	playerE2 = sceneInstance.GetCurrentScene()->CreateObject("PlayerE2");
	playerE2->GetComponent<Transform>()->SetPosition(-68, 7, 7);
	playerE2->AddComponent<Particle>();
	playerE2->GetComponent<Particle>()->SetParticleEffect("Halo1", "Resources/Textures/Particles/fx_Halo1.dds", 1000);
	playerE2->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	playerE2->GetComponent<Particle>()->SetParticleVelocity(30.0f, true);
	playerE2->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	playerE2->GetComponent<Particle>()->AddParticleColor(2.0f, 0.4f, 0.0f);
	playerE2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	playerE2->GetComponent<Particle>()->SetParticleCameraApply(true);

	playerE3 = sceneInstance.GetCurrentScene()->CreateObject("PlayerE3");
	playerE3->GetComponent<Transform>()->SetPosition(-68, 7, 7.f);
	playerE3->AddComponent<Particle>();
	playerE3->GetComponent<Particle>()->SetParticleEffect("Flare6", "Resources/Textures/Particles/fx_Twister3.dds", 1000);
	playerE3->GetComponent<Particle>()->SetParticleDuration(0.1f, 0.1f);
	playerE3->GetComponent<Particle>()->SetParticleVelocity(4.5f, true);
	playerE3->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	playerE3->GetComponent<Particle>()->AddParticleColor(2.0f, 0.0f, 0.0f);
	playerE3->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	playerE3->GetComponent<Particle>()->SetParticleAngle(335.f, 0.0f, 30.f);
	playerE3->GetComponent<Particle>()->SetParticleCameraApply(true);

	playerE2_1 = sceneInstance.GetCurrentScene()->CreateObject("PlayerE2_1");
	playerE2_1->GetComponent<Transform>()->SetPosition(-115, 10.2, -65.f);
	playerE2_1->AddComponent<Particle>();
	playerE2_1->GetComponent<Particle>()->SetParticleEffect("Halo1", "Resources/Textures/Particles/fx_Halo1.dds", 1000);
	playerE2_1->GetComponent<Particle>()->SetParticleDuration(0.5f, 0.2f);
	playerE2_1->GetComponent<Particle>()->SetParticleVelocity(50.0f, true);
	playerE2_1->GetComponent<Particle>()->SetParticleSize(15.f, 15.0f);
	playerE2_1->GetComponent<Particle>()->AddParticleColor(2.0f, 0.8f, 0.0f);
	playerE2_1->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	playerE2_1->GetComponent<Particle>()->SetParticleAngle(335.f, 0.0f, 30.f);
	playerE2_1->GetComponent<Particle>()->SetParticleCameraApply(true);

	playerE2_2 = sceneInstance.GetCurrentScene()->CreateObject("PlayerE2_2");
	playerE2_2->GetComponent<Transform>()->SetPosition(-115, 10.2, -65.f);
	playerE2_2->AddComponent<Particle>();
	playerE2_2->GetComponent<Particle>()->SetParticleEffect("BlastWave2", "Resources/Textures/Particles/fx_BlastWave2.dds", 1000);
	playerE2_2->GetComponent<Particle>()->SetParticleDuration(0.2f, 0.2f);
	playerE2_2->GetComponent<Particle>()->SetParticleVelocity(30.0f, true);
	playerE2_2->GetComponent<Particle>()->SetParticleSize(15.f, 15.0f);
	playerE2_2->GetComponent<Particle>()->AddParticleColor(1.0f, 0.3f, 0.0f);
	playerE2_2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	playerE2_2->GetComponent<Particle>()->SetParticleAngle(339, 0, 30);
	playerE2_2->GetComponent<Particle>()->SetParticleCameraApply(true);

	playerE2_3 = sceneInstance.GetCurrentScene()->CreateObject("PlayerE2_3");
	playerE2_3->GetComponent<Transform>()->SetPosition(-115, 10.2, -65.f);
	playerE2_3->AddComponent<Particle>();
	playerE2_3->GetComponent<Particle>()->SetParticleEffect("Flare6", "Resources/Textures/Particles/fx_Twister3.dds", 1000);
	playerE2_3->GetComponent<Particle>()->SetParticleDuration(0.2f, 0.2f);
	playerE2_3->GetComponent<Particle>()->SetParticleVelocity(20.0f, true);
	playerE2_3->GetComponent<Particle>()->SetParticleSize(22.f, 22.0f);
	playerE2_3->GetComponent<Particle>()->AddParticleColor(1.0f, 0.0f, 0.0f);
	playerE2_3->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	playerE2_3->GetComponent<Particle>()->SetParticleCameraApply(true);
	playerE2_3->GetComponent<Particle>()->SetParticleAngle(339, 0, 30);

	bossTeleportAttack1 = sceneInstance.GetCurrentScene()->CreateObject("bossTeleportAttack1");
	bossTeleportAttack1->GetComponent<Transform>()->SetPosition(48, 10.2, 125.f);
	bossTeleportAttack1->AddComponent<Particle>();
	bossTeleportAttack1->GetComponent<Particle>()->SetParticleEffect("BlastWave1", "Resources/Textures/Particles/fx_BlastWave1.dds", 1000);
	bossTeleportAttack1->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.2f);
	bossTeleportAttack1->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	bossTeleportAttack1->GetComponent<Particle>()->SetParticleSize(15.f, 15.0f);
	bossTeleportAttack1->GetComponent<Particle>()->AddParticleColor(0.05f, 0.1f, 0.0f);
	bossTeleportAttack1->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	//_particleTeleportList

	bossTeleportAttack2 = sceneInstance.GetCurrentScene()->CreateObject("bossTeleportAttack2");
	bossTeleportAttack2->GetComponent<Transform>()->SetPosition(48, 10.2, 125.f);
	bossTeleportAttack2->AddComponent<Particle>();
	bossTeleportAttack2->GetComponent<Particle>()->SetParticleEffect("fx_BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
	bossTeleportAttack2->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.2f);
	bossTeleportAttack2->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	bossTeleportAttack2->GetComponent<Particle>()->SetParticleSize(15.f, 15.0f);
	bossTeleportAttack2->GetComponent<Particle>()->AddParticleColor(0.1f, 0.1f, 0.04f);
	bossTeleportAttack2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	bossTeleportAttack2->GetComponent<Particle>()->SetParticleAngle(80.0f, 0.0f, 0.0f);

	bossTeleportAttack3 = sceneInstance.GetCurrentScene()->CreateObject("bossTeleportAttack3");
	bossTeleportAttack3->GetComponent<Transform>()->SetPosition(48, 10.2, 125.f);
	bossTeleportAttack3->AddComponent<Particle>();
	bossTeleportAttack3->GetComponent<Particle>()->SetParticleEffect("fx_BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
	bossTeleportAttack3->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.2f);
	bossTeleportAttack3->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	bossTeleportAttack3->GetComponent<Particle>()->SetParticleSize(15.f, 15.0f);
	bossTeleportAttack3->GetComponent<Particle>()->AddParticleColor(0.1f, 0.1f, 0.04f);
	bossTeleportAttack3->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	bossTeleportAttack3->GetComponent<Particle>()->SetParticleAngle(260.0f, 0.0f, 0.0f);

	bossTeleportAttack4 = sceneInstance.GetCurrentScene()->CreateObject("bossTeleportAttack4");
	bossTeleportAttack4->GetComponent<Transform>()->SetPosition(48, 10.2, 125.f);
	bossTeleportAttack4->AddComponent<Particle>();
	bossTeleportAttack4->GetComponent<Particle>()->SetParticleEffect("Halo2", "Resources/Textures/Particles/fx_Halo2.dds", 1000);
	bossTeleportAttack4->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.2f);
	bossTeleportAttack4->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	bossTeleportAttack4->GetComponent<Particle>()->SetParticleSize(15.f, 15.0f);
	bossTeleportAttack4->GetComponent<Particle>()->AddParticleColor(0.1f, 0.1f, 0.0f);
	bossTeleportAttack4->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	// q 폭발

	playerQEnd1 = sceneInstance.GetCurrentScene()->CreateObject("PlayerQ1End1");
	playerQEnd1->GetComponent<Transform>()->SetPosition(35, 0, -110);
	playerQEnd1->AddComponent<Particle>();
	playerQEnd1->GetComponent<Particle>()->SetParticleEffect("BlastWave1", "Resources/Textures/Particles/fx_BlastWave1.dds", 1000);
	playerQEnd1->GetComponent<Particle>()->SetParticleDuration(0.1f, 0.1f);
	playerQEnd1->GetComponent<Particle>()->SetParticleVelocity(100.0f, true);
	playerQEnd1->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	playerQEnd1->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 1.0f);
	playerQEnd1->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	playerQEnd2 = sceneInstance.GetCurrentScene()->CreateObject("PlayerQ1End2");
	playerQEnd2->GetComponent<Transform>()->SetPosition(35, 0, -110);
	playerQEnd2->AddComponent<Particle>();
	playerQEnd2->GetComponent<Particle>()->SetParticleEffect("Cracks1", "Resources/Textures/Particles/fx_Cracks1.dds", 1000);
	playerQEnd2->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	playerQEnd2->GetComponent<Particle>()->SetParticleVelocity(150.0f, true);
	playerQEnd2->GetComponent<Particle>()->SetParticleSize(3.0f, 3.0f);
	playerQEnd2->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 2.0f);
	playerQEnd2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);


	playerQEnd3 = sceneInstance.GetCurrentScene()->CreateObject("PlayerQ1End3");
	playerQEnd3->GetComponent<Transform>()->SetPosition(35, 0, -110);
	playerQEnd3->AddComponent<Particle>();
	playerQEnd3->GetComponent<Particle>()->SetParticleEffect("Halo2", "Resources/Textures/Particles/fx_Halo2.dds", 1000);
	playerQEnd3->GetComponent<Particle>()->SetParticleDuration(0.1f, 0.1f);
	playerQEnd3->GetComponent<Particle>()->SetParticleVelocity(100.0f, true);
	playerQEnd3->GetComponent<Particle>()->SetParticleSize(15.f, 15.0f);
	playerQEnd3->GetComponent<Particle>()->AddParticleColor(0.0f, 1.0f, 5.0f);
	playerQEnd3->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	// r 폭발
	playerREnd1 = sceneInstance.GetCurrentScene()->CreateObject("PlayerREnd1");
	playerREnd1->GetComponent<Transform>()->SetPosition(-5, 0, -110);
	playerREnd1->AddComponent<Particle>();
	playerREnd1->GetComponent<Particle>()->SetParticleEffect("Flare6", "Resources/Textures/Particles/fx_Twister3.dds", 1000);
	playerREnd1->GetComponent<Particle>()->SetParticleDuration(0.6f, 0.5f);
	playerREnd1->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	playerREnd1->GetComponent<Particle>()->SetParticleSize(15.f, 15.f);
	playerREnd1->GetComponent<Particle>()->AddParticleColor(1.f, 1.f, 1.f);
	playerREnd1->GetComponent<Particle>()->SetParticleDirection(0.0f, 200.0f, 0.0f);

	playerREnd2 = sceneInstance.GetCurrentScene()->CreateObject("PlayerREnd2");
	playerREnd2->GetComponent<Transform>()->SetPosition(-5, 0, -110);
	playerREnd2->AddComponent<Particle>();
	playerREnd2->GetComponent<Particle>()->SetParticleEffect("Halo1", "Resources/Textures/Particles/fx_Halo1.dds", 1000);
	playerREnd2->GetComponent<Particle>()->SetParticleDuration(2.0f, 0.7f);
	playerREnd2->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	playerREnd2->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	playerREnd2->GetComponent<Particle>()->AddParticleColor(2.0f, 0.5f, 0.0f);
	playerREnd2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	playerREnd2->GetComponent<Particle>()->SetParticleCameraApply(true);

	playerREnd3 = sceneInstance.GetCurrentScene()->CreateObject("PlayerREnd3");
	playerREnd3->GetComponent<Transform>()->SetPosition(-5, 0, -110);
	playerREnd3->AddComponent<Particle>();
	playerREnd3->GetComponent<Particle>()->SetParticleEffect("Flare6", "Resources/Textures/Particles/fx_Twister3.dds", 1000);
	playerREnd3->GetComponent<Particle>()->SetParticleDuration(0.6f, 0.5f);
	playerREnd3->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	playerREnd3->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	playerREnd3->GetComponent<Particle>()->AddParticleColor(2.0f, 0.3f, 0.1f);
	playerREnd3->GetComponent<Particle>()->SetParticleDirection(0.0f, 300.0f, 0.0f);
	playerREnd3->GetComponent<Particle>()->SetParticleCameraApply(true);

	playerREnd4 = sceneInstance.GetCurrentScene()->CreateObject("PlayerREnd4");
	playerREnd4->GetComponent<Transform>()->SetPosition(-5, 0, -110);
	playerREnd4->AddComponent<Particle>();
	playerREnd4->GetComponent<Particle>()->SetParticleEffect("BlastWave2", "Resources/Textures/Particles/fx_BlastWave2.dds", 1000);
	playerREnd4->GetComponent<Particle>()->SetParticleDuration(1.0f, 3.0f);
	playerREnd4->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
	playerREnd4->GetComponent<Particle>()->SetParticleSize(45.f, 45.0f);
	playerREnd4->GetComponent<Particle>()->AddParticleColor(1.0f, 0.0f, 0.0f);
	playerREnd4->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	playerREnd4->GetComponent<Particle>()->SetParticleCameraApply(true);

	// 맵 꾸미기 파티클

	mapParticle1 = sceneInstance.GetCurrentScene()->CreateObject("MapParticle1");
	mapParticle1->GetComponent<Transform>()->SetPosition(-113.83, 10.31, -136.35f);
	mapParticle1->AddComponent<Particle>();
	mapParticle1->GetComponent<Particle>()->SetParticleEffect("Flare5", "Resources/Textures/Particles/fx_Twister2.dds", 1000);
	mapParticle1->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.5f);
	mapParticle1->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	mapParticle1->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	mapParticle1->GetComponent<Particle>()->AddParticleColor(0.3f, 10.0f, 0.0f);
	mapParticle1->GetComponent<Particle>()->SetParticleDirection(0.0f, 20.0f, 0.0f);
	mapParticle1->GetComponent<Particle>()->SetParticleCameraApply(true);

	mapParticle2 = sceneInstance.GetCurrentScene()->CreateObject("MapParticle2");
	mapParticle2->GetComponent<Transform>()->SetPosition(-38.6, 3.88, -140.8f);
	mapParticle2->AddComponent<Particle>();
	mapParticle2->GetComponent<Particle>()->SetParticleEffect("Flare5", "Resources/Textures/Particles/fx_Twister2.dds", 1000);
	mapParticle2->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.5f);
	mapParticle2->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	mapParticle2->GetComponent<Particle>()->SetParticleSize(8.f, 8.0f);
	mapParticle2->GetComponent<Particle>()->AddParticleColor(3.0f, 10.0f, 1.0f);
	mapParticle2->GetComponent<Particle>()->SetParticleDirection(0.0f, 40.0f, 0.0f);
	mapParticle2->GetComponent<Particle>()->SetParticleCameraApply(true);

	mapParticle3 = sceneInstance.GetCurrentScene()->CreateObject("MapParticle3");
	mapParticle3->GetComponent<Transform>()->SetPosition(39.18f, 3.88, -140.8f);
	mapParticle3->AddComponent<Particle>();
	mapParticle3->GetComponent<Particle>()->SetParticleEffect("Flare5", "Resources/Textures/Particles/fx_Twister2.dds", 1000);
	mapParticle3->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.5f);
	mapParticle3->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	mapParticle3->GetComponent<Particle>()->SetParticleSize(8.f, 8.0f);
	mapParticle3->GetComponent<Particle>()->AddParticleColor(3.0f, 10.0f, 1.0f);
	mapParticle3->GetComponent<Particle>()->SetParticleDirection(0.0f, 40.0f, 0.0f);
	mapParticle3->GetComponent<Particle>()->SetParticleCameraApply(true);

	mapParticle4 = sceneInstance.GetCurrentScene()->CreateObject("MapParticle4");
	mapParticle4->GetComponent<Transform>()->SetPosition(115.42, 10.31, -136.35f);
	mapParticle4->AddComponent<Particle>();
	mapParticle4->GetComponent<Particle>()->SetParticleEffect("Flare5", "Resources/Textures/Particles/fx_Twister2.dds", 1000);
	mapParticle4->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.5f);
	mapParticle4->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	mapParticle4->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	mapParticle4->GetComponent<Particle>()->AddParticleColor(0.3f, 10.0f, 0.0f);
	mapParticle4->GetComponent<Particle>()->SetParticleDirection(0.0f, 20.0f, 0.0f);
	mapParticle4->GetComponent<Particle>()->SetParticleCameraApply(true);

	mapParticle5 = sceneInstance.GetCurrentScene()->CreateObject("MapParticle5");
	mapParticle5->GetComponent<Transform>()->SetPosition(115.42f, 11.316f, 124.14f);
	mapParticle5->AddComponent<Particle>();
	mapParticle5->GetComponent<Particle>()->SetParticleEffect("Flare5", "Resources/Textures/Particles/fx_Twister2.dds", 1000);
	mapParticle5->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.5f);
	mapParticle5->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	mapParticle5->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	mapParticle5->GetComponent<Particle>()->AddParticleColor(0.3f, 10.0f, 0.0f);
	mapParticle5->GetComponent<Particle>()->SetParticleDirection(0.0f, 20.0f, 0.0f);
	mapParticle5->GetComponent<Particle>()->SetParticleCameraApply(true);

	mapParticle6 = sceneInstance.GetCurrentScene()->CreateObject("MapParticle6");
	mapParticle6->GetComponent<Transform>()->SetPosition(28.16f, 37.125f, 138.95f);
	mapParticle6->AddComponent<Particle>();
	mapParticle6->GetComponent<Particle>()->SetParticleEffect("Flare5", "Resources/Textures/Particles/fx_Twister2.dds", 1000);
	mapParticle6->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.6f);
	mapParticle6->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	mapParticle6->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	mapParticle6->GetComponent<Particle>()->AddParticleColor(1.0f, 10.0f, 0.0f);
	mapParticle6->GetComponent<Particle>()->SetParticleDirection(0.0f, 20.0f, 0.0f);
	mapParticle6->GetComponent<Particle>()->SetParticleCameraApply(true);

	mapParticle7 = sceneInstance.GetCurrentScene()->CreateObject("MapParticle7");
	mapParticle7->GetComponent<Transform>()->SetPosition(-27.11f, 37.125f, 138.95f);
	mapParticle7->AddComponent<Particle>();
	mapParticle7->GetComponent<Particle>()->SetParticleEffect("Flare5", "Resources/Textures/Particles/fx_Twister2.dds", 1000);
	mapParticle7->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.6f);
	mapParticle7->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	mapParticle7->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	mapParticle7->GetComponent<Particle>()->AddParticleColor(1.0f, 10.0f, 0.0f);
	mapParticle7->GetComponent<Particle>()->SetParticleDirection(0.0f, 20.0f, 0.0f);
	mapParticle7->GetComponent<Particle>()->SetParticleCameraApply(true);

	mapParticle8 = sceneInstance.GetCurrentScene()->CreateObject("MapParticle8");
	mapParticle8->GetComponent<Transform>()->SetPosition(-113.37f, 11.316f, 125.89f);
	mapParticle8->AddComponent<Particle>();
	mapParticle8->GetComponent<Particle>()->SetParticleEffect("Flare5", "Resources/Textures/Particles/fx_Twister2.dds", 1000);
	mapParticle8->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.5f);
	mapParticle8->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	mapParticle8->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	mapParticle8->GetComponent<Particle>()->AddParticleColor(0.3f, 10.0f, 0.0f);
	mapParticle8->GetComponent<Particle>()->SetParticleDirection(0.0f, 20.0f, 0.0f);
	mapParticle8->GetComponent<Particle>()->SetParticleCameraApply(true);

	mapParticleEye1 = sceneInstance.GetCurrentScene()->CreateObject("MapParticleEye1");
	mapParticleEye1->GetComponent<Transform>()->SetPosition(122.3f, 18.13f, -5.02f);
	mapParticleEye1->AddComponent<Particle>();
	mapParticleEye1->GetComponent<Particle>()->SetParticleEffect("Lightning1", "Resources/Textures/Particles/fx_Lightning1.dds", 1000);
	mapParticleEye1->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	mapParticleEye1->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
	mapParticleEye1->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	mapParticleEye1->GetComponent<Particle>()->AddParticleColor(1.0f, 0.1f, 0.0f);
	mapParticleEye1->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	mapParticleEye2 = sceneInstance.GetCurrentScene()->CreateObject("MapParticleEye2");
	mapParticleEye2->GetComponent<Transform>()->SetPosition(64.84f, 42.395f, 215.4f);
	mapParticleEye2->AddComponent<Particle>();
	mapParticleEye2->GetComponent<Particle>()->SetParticleEffect("Lightning1", "Resources/Textures/Particles/fx_Lightning1.dds", 1000);
	mapParticleEye2->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	mapParticleEye2->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
	mapParticleEye2->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	mapParticleEye2->GetComponent<Particle>()->AddParticleColor(1.0f, 0.1f, 0.0f);
	mapParticleEye2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	mapParticleEye3 = sceneInstance.GetCurrentScene()->CreateObject("MapParticleEye3");
	mapParticleEye3->GetComponent<Transform>()->SetPosition(42.37f, 19, 144.6f);
	mapParticleEye3->AddComponent<Particle>();
	mapParticleEye3->GetComponent<Particle>()->SetParticleEffect("Blaster1", "Resources/Textures/Particles/fx_Blaster1.dds", 1000);
	mapParticleEye3->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	mapParticleEye3->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
	mapParticleEye3->GetComponent<Particle>()->SetParticleSize(3.f, 1.0f);
	mapParticleEye3->GetComponent<Particle>()->AddParticleColor(10.0f, 0.1f, 0.0f);
	mapParticleEye3->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	mapParticleEye3->GetComponent<Particle>()->SetParticleAngle(0.0f, 0.0f, 356.0f);

	mapParticleEye4 = sceneInstance.GetCurrentScene()->CreateObject("MapParticleEye4");
	mapParticleEye4->GetComponent<Transform>()->SetPosition(37.45f, 19, 144.6f);
	mapParticleEye4->AddComponent<Particle>();
	mapParticleEye4->GetComponent<Particle>()->SetParticleEffect("Blaster1", "Resources/Textures/Particles/fx_Blaster1.dds", 1000);
	mapParticleEye4->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	mapParticleEye4->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
	mapParticleEye4->GetComponent<Particle>()->SetParticleSize(3.f, 1.0f);
	mapParticleEye4->GetComponent<Particle>()->AddParticleColor(10.0f, 0.4f, 0.0f);
	mapParticleEye4->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	mapParticleEye4->GetComponent<Particle>()->SetParticleAngle(0.0f, 0.0f, 30.0f);

	mapParticleEye5 = sceneInstance.GetCurrentScene()->CreateObject("MapParticleEye5");
	mapParticleEye5->GetComponent<Transform>()->SetPosition(-63.866f, 42.395f, 215.93f);
	mapParticleEye5->AddComponent<Particle>();
	mapParticleEye5->GetComponent<Particle>()->SetParticleEffect("Lightning1", "Resources/Textures/Particles/fx_Lightning1.dds", 1000);
	mapParticleEye5->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	mapParticleEye5->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
	mapParticleEye5->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	mapParticleEye5->GetComponent<Particle>()->AddParticleColor(1.0f, 0.1f, 0.0f);
	mapParticleEye5->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	mapParticleEye6 = sceneInstance.GetCurrentScene()->CreateObject("MapParticleEye6");
	mapParticleEye6->GetComponent<Transform>()->SetPosition(-36.433f, 19, 144.6f);
	mapParticleEye6->AddComponent<Particle>();
	mapParticleEye6->GetComponent<Particle>()->SetParticleEffect("Blaster1", "Resources/Textures/Particles/fx_Blaster1.dds", 1000);
	mapParticleEye6->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	mapParticleEye6->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
	mapParticleEye6->GetComponent<Particle>()->SetParticleSize(3.f, 1.0f);
	mapParticleEye6->GetComponent<Particle>()->AddParticleColor(10.0f, 0.1f, 0.0f);
	mapParticleEye6->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	mapParticleEye6->GetComponent<Particle>()->SetParticleAngle(0.0f, 0.0f, 356.0f);

	mapParticleEye7 = sceneInstance.GetCurrentScene()->CreateObject("MapParticleEye7");
	mapParticleEye7->GetComponent<Transform>()->SetPosition(-41.3f, 19, 144.6f);
	mapParticleEye7->AddComponent<Particle>();
	mapParticleEye7->GetComponent<Particle>()->SetParticleEffect("Blaster1", "Resources/Textures/Particles/fx_Blaster1.dds", 1000);
	mapParticleEye7->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	mapParticleEye7->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
	mapParticleEye7->GetComponent<Particle>()->SetParticleSize(3.f, 1.0f);
	mapParticleEye7->GetComponent<Particle>()->AddParticleColor(10.0f, 0.4f, 0.0f);
	mapParticleEye7->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	mapParticleEye7->GetComponent<Particle>()->SetParticleAngle(0.0f, 0.0f, 30.0f);

	mapParticleEye8 = sceneInstance.GetCurrentScene()->CreateObject("MapParticleEye8");
	mapParticleEye8->GetComponent<Transform>()->SetPosition(-120.72f, 18.02f, -5.02f);
	mapParticleEye8->AddComponent<Particle>();
	mapParticleEye8->GetComponent<Particle>()->SetParticleEffect("Lightning1", "Resources/Textures/Particles/fx_Lightning1.dds", 1000);
	mapParticleEye8->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	mapParticleEye8->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
	mapParticleEye8->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	mapParticleEye8->GetComponent<Particle>()->AddParticleColor(1.0f, 0.1f, 0.0f);
	mapParticleEye8->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	//bossEgoRush1 = sceneInstance.GetCurrentScene()->CreateObject("BossEgoRush1");
	//bossEgoRush1->AddComponent<Particle>();
	//bossEgoRush1->GetComponent<Particle>()->SetParticleEffect("Lightning2", "Resources/Textures/Particles/fx_Lightning2.dds", 1000);
	//bossEgoRush1->GetComponent<Particle>()->SetParticleDuration(0.4f, 2.0f);
	//bossEgoRush1->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	//bossEgoRush1->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	//bossEgoRush1->GetComponent<Particle>()->AddParticleColor(1.0f, 6.f, 0.0f);
	//bossEgoRush1->GetComponent<Particle>()->SetParticleDirection(0.0f, 40.0f, 0.0f);
	//bossEgoRush1->SetParent(kamen);
	//bossEgoRush1->GetComponent<Transform>()->SetPosition(-0.272f, 18.527f, -3.0f);

	//bossEgoRush2 = sceneInstance.GetCurrentScene()->CreateObject("BossEgoRush2");
	//bossEgoRush2->AddComponent<Particle>();
	//bossEgoRush2->GetComponent<Particle>()->SetParticleEffect("fx_BlastWave5", "Resources/Textures/Particles/fx_BlastWave5.dds", 1000);
	//bossEgoRush2->GetComponent<Particle>()->SetParticleDuration(2.0f, 4.0f);
	//bossEgoRush2->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	//bossEgoRush2->GetComponent<Particle>()->SetParticleSize(15.0f, 40.0f);
	//bossEgoRush2->GetComponent<Particle>()->AddParticleColor(0.5f, 6.0f, 0.0f);
	//bossEgoRush2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	//bossEgoRush2->SetParent(kamen);
	//bossEgoRush2->GetComponent<Transform>()->SetPosition(0, 13.0f, 0.0f);

	//bossEgoRush3 = sceneInstance.GetCurrentScene()->CreateObject("BossEgoRush3");
	//bossEgoRush3->AddComponent<Particle>();
	//bossEgoRush3->GetComponent<Particle>()->SetParticleEffect("fx_BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
	//bossEgoRush3->GetComponent<Particle>()->SetParticleDuration(2.0f, 4.0f);
	//bossEgoRush3->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	//bossEgoRush3->GetComponent<Particle>()->SetParticleSize(15.0f, 40.0f);
	//bossEgoRush3->GetComponent<Particle>()->AddParticleColor(0.5f, 6.0f, 0.0f);
	//bossEgoRush3->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	//bossEgoRush3->SetParent(kamen);
	//bossEgoRush3->GetComponent<Transform>()->SetPosition(0, 10.0f, -2.0f);

	//bossEgoRush4 = sceneInstance.GetCurrentScene()->CreateObject("BossEgoRush4");
	//bossEgoRush4->AddComponent<Particle>();
	//bossEgoRush4->GetComponent<Particle>()->SetParticleEffect("Lightning2", "Resources/Textures/Particles/fx_Lightning2.dds", 1000);
	//bossEgoRush4->GetComponent<Particle>()->SetParticleDuration(0.5f, 1.0f);
	//bossEgoRush4->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	//bossEgoRush4->GetComponent<Particle>()->SetParticleSize(15.f, 40.0f);
	//bossEgoRush4->GetComponent<Particle>()->AddParticleColor(0.6f, 6.0f, 0.0f);
	//bossEgoRush4->GetComponent<Particle>()->SetParticleDirection(0.0f, 70.0f, 0.0f);
	//bossEgoRush3->SetParent(kamen);
	//bossEgoRush4->GetComponent<Transform>()->SetPosition(0, 13.4, 0);

	// 3페이즈 보스 이펙트, 링부분 앵글에 보스의 로테이션 Y값을 받아줘야함
	// 뒷면에는 180을 더 한 값을 받아줘야함

	//boss3Phase1 = sceneInstance.GetCurrentScene()->CreateObject("Boss3Phase1");
	//boss3Phase1->AddComponent<Particle>();
	//boss3Phase1->GetComponent<Particle>()->SetParticleEffect("Lightning2", "Resources/Textures/Particles/fx_Lightning2.dds", 1000);
	//boss3Phase1->GetComponent<Particle>()->SetParticleDuration(0.4f, 2.0f);
	//boss3Phase1->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	//boss3Phase1->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	//boss3Phase1->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	//boss3Phase1->GetComponent<Particle>()->SetParticleDirection(0.0f, 40.0f, 0.0f);
	//boss3Phase1->SetParent(kamen);
	//boss3Phase1->GetComponent<Transform>()->SetPosition(-0.272f, 18.527f, -3.0f);
	//
	//boss3Phase2 = sceneInstance.GetCurrentScene()->CreateObject("Boss3Phase2");
	//boss3Phase2->AddComponent<Particle>();
	//boss3Phase2->GetComponent<Particle>()->SetParticleEffect("Halo1", "Resources/Textures/Particles/fx_Halo1.dds", 1000);
	//boss3Phase2->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.3f);
	//boss3Phase2->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
	//boss3Phase2->GetComponent<Particle>()->SetParticleSize(35.f, 35.0f);
	//boss3Phase2->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	//boss3Phase2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	//boss3Phase2->GetComponent<Particle>()->SetParticleAngle(270.0f, 0.0f, 0.0f);
	//boss3Phase2->GetComponent<Particle>()->SetParticleCameraApply(true);
	//boss3Phase2->SetParent(kamen);
	//boss3Phase2->GetComponent<Transform>()->SetPosition(0.0f, 18.f, 3.f);
	//
	//boss3Phase2_2 = sceneInstance.GetCurrentScene()->CreateObject("Boss3Phase2_2");
	//boss3Phase2_2->AddComponent<Particle>();
	//boss3Phase2_2->GetComponent<Particle>()->SetParticleEffect("Halo1", "Resources/Textures/Particles/fx_Halo1.dds", 1000);
	//boss3Phase2_2->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.3f);
	//boss3Phase2_2->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
	//boss3Phase2_2->GetComponent<Particle>()->SetParticleSize(35.f, 35.0f);
	//boss3Phase2_2->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	//boss3Phase2_2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	//boss3Phase2_2->GetComponent<Particle>()->SetParticleAngle(270.0f, 180.0f, 0.0f);
	//boss3Phase2_2->GetComponent<Particle>()->SetParticleCameraApply(true);
	//boss3Phase2_2->SetParent(kamen);
	//boss3Phase2_2->GetComponent<Transform>()->SetPosition(0.0f, 18.f, 3.f);
	//
	//boss3Phase3 = sceneInstance.GetCurrentScene()->CreateObject("Boss3Phase3");
	//boss3Phase3->AddComponent<Particle>();
	//boss3Phase3->GetComponent<Particle>()->SetParticleEffect("BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
	//boss3Phase3->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.0f);
	//boss3Phase3->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	//boss3Phase3->GetComponent<Particle>()->SetParticleSize(25.f, 25.0f);
	//boss3Phase3->GetComponent<Particle>()->AddParticleColor(6.0f, 0.2f, 0.0f);
	//boss3Phase3->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	//boss3Phase3->GetComponent<Particle>()->SetParticleAngle(270.0f, 0, 0);
	//boss3Phase3->GetComponent<Particle>()->SetParticleCameraApply(true);
	//boss3Phase3->SetParent(kamen);
	//boss3Phase3->GetComponent<Transform>()->SetPosition(0, 18.f, 3.f);
	//
	//boss3Phase3_2 = sceneInstance.GetCurrentScene()->CreateObject("Boss3Phase3_2");
	//boss3Phase3_2->AddComponent<Particle>();
	//boss3Phase3_2->GetComponent<Particle>()->SetParticleEffect("BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
	//boss3Phase3_2->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.0f);
	//boss3Phase3_2->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	//boss3Phase3_2->GetComponent<Particle>()->SetParticleSize(25.f, 25.0f);
	//boss3Phase3_2->GetComponent<Particle>()->AddParticleColor(6.0f, 0.2f, 0.0f);
	//boss3Phase3_2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	//boss3Phase3_2->GetComponent<Particle>()->SetParticleAngle(270.0f, 180, 0);
	//boss3Phase3_2->GetComponent<Particle>()->SetParticleCameraApply(true);
	//boss3Phase3_2->SetParent(kamen);
	//boss3Phase3_2->GetComponent<Transform>()->SetPosition(0, 18.f, 3.f);

	bossCoreBladeMove1 = sceneInstance.GetCurrentScene()->CreateObject("bossCoreBladeMove1");
	bossCoreBladeMove1->AddComponent<Particle>();
	bossCoreBladeMove1->GetComponent<Particle>()->SetParticleEffect("fx_Lightning1", "Resources/Textures/Particles/fx_Lightning1.dds", 1000);
	bossCoreBladeMove1->GetComponent<Particle>()->SetParticleDuration(7.0f, 3.0f);
	bossCoreBladeMove1->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	bossCoreBladeMove1->GetComponent<Particle>()->SetParticleSize(1.f, 1.0f);
	bossCoreBladeMove1->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	bossCoreBladeMove1->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	bossCoreBladeMove1->GetComponent<Particle>()->SetParticleCameraApply(true);
	bossCoreBladeMove1->SetParent(sword);
	bossCoreBladeMove1->GetComponent<Transform>()->SetPosition(0, 21.9f, -1.2);
	
	bossCoreBladeMove2 = sceneInstance.GetCurrentScene()->CreateObject("bossCoreBladeMove2");
	bossCoreBladeMove2->AddComponent<Particle>();
	bossCoreBladeMove2->GetComponent<Particle>()->SetParticleEffect("fx_Lightning3", "Resources/Textures/Particles/fx_Lightning3.dds", 1000);
	bossCoreBladeMove2->GetComponent<Particle>()->SetParticleDuration(0.5f, 2.0f);
	bossCoreBladeMove2->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	bossCoreBladeMove2->GetComponent<Particle>()->SetParticleSize(7.f, 7.0f);
	bossCoreBladeMove2->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	bossCoreBladeMove2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	bossCoreBladeMove2->GetComponent<Particle>()->SetParticleAngle(0.0f, 0.0f, 0.0f);
	bossCoreBladeMove2->GetComponent<Particle>()->SetParticleCameraApply(true);
	bossCoreBladeMove2->SetParent(sword);
	bossCoreBladeMove2->GetComponent<Transform>()->SetPosition(-0.1f, 23.0f, -8.0f);
	
	bossCoreBladeMove3 = sceneInstance.GetCurrentScene()->CreateObject("bossCoreBladeMove3");
	bossCoreBladeMove3->AddComponent<Particle>();
	bossCoreBladeMove3->GetComponent<Particle>()->SetParticleEffect("fx_Lightning3", "Resources/Textures/Particles/fx_Lightning3.dds", 1000);
	bossCoreBladeMove3->GetComponent<Particle>()->SetParticleDuration(0.5f, 2.0f);
	bossCoreBladeMove3->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	bossCoreBladeMove3->GetComponent<Particle>()->SetParticleSize(7.f, 7.0f);
	bossCoreBladeMove3->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	bossCoreBladeMove3->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	bossCoreBladeMove3->GetComponent<Particle>()->SetParticleAngle(0.0f, 0.0f, 0.0f);
	bossCoreBladeMove3->GetComponent<Particle>()->SetParticleCameraApply(true);
	bossCoreBladeMove3->SetParent(sword);
	bossCoreBladeMove3->GetComponent<Transform>()->SetPosition(-0.1f, 23.0f, -4.5f);
	
	bossCoreBladeMove4 = sceneInstance.GetCurrentScene()->CreateObject("bossCoreBladeMove4");
	bossCoreBladeMove4->AddComponent<Particle>();
	bossCoreBladeMove4->GetComponent<Particle>()->SetParticleEffect("fx_Lightning3", "Resources/Textures/Particles/fx_Lightning3.dds", 1000);
	bossCoreBladeMove4->GetComponent<Particle>()->SetParticleDuration(0.5f, 2.0f);
	bossCoreBladeMove4->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	bossCoreBladeMove4->GetComponent<Particle>()->SetParticleSize(7.f, 7.0f);
	bossCoreBladeMove4->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	bossCoreBladeMove4->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	bossCoreBladeMove4->GetComponent<Particle>()->SetParticleAngle(0.0f, 0.0f, 0.0f);
	bossCoreBladeMove4->GetComponent<Particle>()->SetParticleCameraApply(true);
	bossCoreBladeMove4->SetParent(sword);
	bossCoreBladeMove4->GetComponent<Transform>()->SetPosition(-0.1f, 23.0f, -0.6);

}

DirectX::XMFLOAT3 KunrealEngine::EngineCore::Bezier(DirectX::XMFLOAT3 startPoint, DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 endPoint, float t)
{
	float u = 1.0f - t;
	float tt = t * t;
	float ttt = tt * t;
	float uu = u * u;
	float uuu = uu * u;

	DirectX::XMFLOAT3 p =
	{
		uuu * startPoint.x + 3 * uu * t * p1.x + 3 * u * tt * p2.x + ttt * endPoint.x,
		uuu * startPoint.y + 3 * uu * t * p1.y + 3 * u * tt * p2.y + ttt * endPoint.y,
		uuu * startPoint.z + 3 * uu * t * p1.z + 3 * u * tt * p2.z + ttt * endPoint.z,
	};

	return p;
}

DirectX::XMFLOAT2 KunrealEngine::EngineCore::BezierPoint2D(DirectX::XMFLOAT2 startPoint, DirectX::XMFLOAT2 p1, DirectX::XMFLOAT2 p2, DirectX::XMFLOAT2 endPoint, float t)
{
	float u = 1.0f - t;
	float tt = t * t;
	float ttt = tt * t;
	float uu = u * u;
	float uuu = uu * u;

	DirectX::XMFLOAT2 p =
	{
		uuu * startPoint.x + 3 * uu * t * p1.x + 3 * u * tt * p2.x + ttt * endPoint.x,
		uuu * startPoint.y + 3 * uu * t * p1.y + 3 * u * tt * p2.y + ttt * endPoint.y,
	};

	return p;
}

std::vector<DirectX::XMFLOAT3> KunrealEngine::EngineCore::BezierSetting(GameObject* bezierObject)
{
	p0.x = bezierObject->GetComponent<Transform>()->GetPosition().x;
	p0.y = bezierObject->GetComponent<Transform>()->GetPosition().y;
	p0.z = bezierObject->GetComponent<Transform>()->GetPosition().z;

	if (p0.x > p3.x)
	{
		p1.x = p0.x - (ToolBox::GetRandomFloat(20.0f, 30.0f) * ToolBox::GetRandomFloat(0.1f, 1.0f));
		p2.x = p0.x - (ToolBox::GetRandomFloat(20.0f, 30.0f) * ToolBox::GetRandomFloat(1.0f, 2.0f));
	}
	else
	{
		p1.x = p0.x + (ToolBox::GetRandomFloat(20.0f, 30.0f) * ToolBox::GetRandomFloat(0.1f, 1.0f));
		p2.x = p0.x + (ToolBox::GetRandomFloat(20.0f, 30.0f) * ToolBox::GetRandomFloat(1.0f, 2.0f));
	}

	p1.y = p0.y + (ToolBox::GetRandomFloat(20.0f, 30.0f) * ToolBox::GetRandomFloat(-1.0f, 1.0f));
	p2.y = p0.y + (ToolBox::GetRandomFloat(20.0f, 30.0f) * ToolBox::GetRandomFloat(-1.0f, 1.0f));

	p1.z = p0.z + (ToolBox::GetRandomFloat(20.0f, 30.0f) * ToolBox::GetRandomFloat(-1.0f, 1.0f));
	p2.z = p0.z + (ToolBox::GetRandomFloat(20.0f, 30.0f) * ToolBox::GetRandomFloat(-1.0f, 1.0f));

	std::vector<DirectX::XMFLOAT3> bezierPosintList;
	bezierPosintList.push_back(p0);
	bezierPosintList.push_back(p1);
	bezierPosintList.push_back(p2);

	//std::vector<std::vector<Point3D>> bezierPointsListSetting;
	//bezierPointsListSetting.push_back(bezierPosintList);

	return bezierPosintList;
}

void KunrealEngine::EngineCore::UpdateParticleTest()
{
	particleBossSword3->GetComponent<Particle>()->SetParticleSize(5.f * ToolBox::GetRandomFloat(0.3f, 1.0f), 5.0f * ToolBox::GetRandomFloat(0.1f, 1.0f));

	POINT particlePoint = { 10,0 };
	POINT particlePoint2 = { 10,0 };
	GameObject* sword = sceneInstance.GetCurrentScene()->GetGameObject("sword2");

	//z 는 조절하지 않는다
	//x값을 신경쓰고 y는 튀어오르는 높이  시작지점이 다르면 y를 -로 줘서 아래로 포물선을 그릴수 있음
	if (_isBezierStartSetting == false)
	{
		//for (auto& bezierObject : _bezierObjectList) // 베지어 곡선 초기 설정
		//{
		//	_bezierPointsList.push_back(BezierSetting(bezierObject));
		//}

		for (auto& ParticleSwordSoul : ParticleSwordSoulList)
		{
			_bezierSwordSoulPointsList.push_back(BezierSetting(ParticleSwordSoul));
		}
		_isBezierStartSetting = true;
	}


	//for (auto& bezierPoint : _bezierPointsList) // 각 점을 통한 베지어 곡선을 구함
	//{	
	//	Point3D particlePoint;
	//	particlePoint = Bezier(bezierPoint[0], bezierPoint[1], bezierPoint[2], p3, _timeCount);
	//	_particlePointList.push_back(particlePoint);
	//}

	int bezierPointIndex = 0;

	for (auto& bezierPoint : _bezierSwordSoulPointsList) // 각 점을 통한 베지어 곡선을 구함
	{
		DirectX::XMFLOAT3 particlePoint;
		DirectX::XMFLOAT3 endPoint;

		endPoint = { sword->GetComponent<Transform>()->GetPosition().x, sword->GetComponent<Transform>()->GetPosition().y + 20.f, sword->GetComponent<Transform>()->GetPosition().z };

		if (_isSettingTimer == false)
		{
			_timeCount = ToolBox::GetRandomFloat(0.1f, 0.8f);

			_timeCountList.push_back(_timeCount);
		}

		particlePoint = Bezier(bezierPoint[0], bezierPoint[1], bezierPoint[2], endPoint, _timeCountList[bezierPointIndex]);
		_particleSwordSoulPointList.push_back(particlePoint);
		++bezierPointIndex;
	}
	_isSettingTimer = true;

	//for (int i = 0; i < _bezierObjectList.size(); ++i) // 곡선을 따라 이동
	//{
	//	_bezierObjectList[i]->GetComponent<Transform>()->SetPosition(_particlePointList[i].x, _particlePointList[i].y, _particlePointList[i].z);
	//}

	for (int i = 0; i < ParticleSwordSoulList.size(); ++i) // 곡선을 따라 이동
	{
		ParticleSwordSoulList[i]->GetComponent<Transform>()->SetPosition(_particleSwordSoulPointList[i].x, _particleSwordSoulPointList[i].y, _particleSwordSoulPointList[i].z);
	}

	//if (_timeCount > 0.7f)
	//{

	// 파티클 업데이트 테스트 동적으로 값변경이 필요할경우 사용

	_timeCount += TimeManager::GetInstance().GetDeltaTime();
	_timeCountPlayerR += TimeManager::GetInstance().GetDeltaTime();

	DirectX::XMFLOAT3 particlePoint3 = { 0,0,0 };
	DirectX::XMFLOAT3 particlePoint3_2 = { 0,0,0 };


	GameObject* test = sceneInstance.GetCurrentScene()->GetGameObject("Particle16");

	test->GetComponent<Particle>()->SetParticleSize(100 * _timeCount, 100 * _timeCount);
	//test->GetComponent<Transform>()->SetRotation(90.0f, 0, 0);
	test->GetComponent<Particle>()->SetParticleRotation(90.0f, 0.0f, 0.0f);

	GameObject* PlayerE1 = sceneInstance.GetCurrentScene()->GetGameObject("PlayerE1");
	PlayerE1->GetComponent<Particle>()->SetParticleSize(20 * _timeCount, 20 * _timeCount);

	GameObject* PlayerE2 = sceneInstance.GetCurrentScene()->GetGameObject("PlayerE2");
	PlayerE2->GetComponent<Particle>()->SetParticleSize(50 * _timeCount, 50 * _timeCount);

	GameObject* PlayerE3 = sceneInstance.GetCurrentScene()->GetGameObject("PlayerE3");
	PlayerE3->GetComponent<Particle>()->SetParticleSize(50 * _timeCount, 50 * _timeCount);

	GameObject* PlayerE2_1 = sceneInstance.GetCurrentScene()->GetGameObject("PlayerE2_1");
	PlayerE2_1->GetComponent<Particle>()->SetParticleSize(20 * _timeCount, 20 * _timeCount);

	GameObject* PlayerE2_2 = sceneInstance.GetCurrentScene()->GetGameObject("PlayerE2_2");
	PlayerE2_2->GetComponent<Particle>()->SetParticleSize(40 * _timeCount, 40 * _timeCount);


	GameObject* PlayerE2_3 = sceneInstance.GetCurrentScene()->GetGameObject("PlayerE2_3");
	PlayerE2_3->GetComponent<Particle>()->SetParticleSize(60 * _timeCount, 60 * _timeCount);

	// 플레이어 Q 착탄

	GameObject* playerQEnd1 = sceneInstance.GetCurrentScene()->GetGameObject("PlayerQ1End1");
	playerQEnd1->GetComponent<Particle>()->SetParticleSize(_timeCount * 40, _timeCount * 40);
	//GameObject* playerQEnd2 = sceneInstance.GetCurrentScene()->GetGameObject("PlayerQ1End2");
	GameObject* playerQEnd3 = sceneInstance.GetCurrentScene()->GetGameObject("PlayerQ1End3");
	playerQEnd3->GetComponent<Particle>()->SetParticleSize(_timeCount * 120, _timeCount * 120);

	GameObject* playerREnd1 = sceneInstance.GetCurrentScene()->GetGameObject("PlayerREnd1");
	playerREnd1->GetComponent<Particle>()->SetParticleSize(15 - (_timeCountPlayerR * 20), 15 - (_timeCountPlayerR * 20));
	GameObject* playerREnd2 = sceneInstance.GetCurrentScene()->GetGameObject("PlayerREnd2");
	GameObject* playerREnd3 = sceneInstance.GetCurrentScene()->GetGameObject("PlayerREnd3");
	playerREnd3->GetComponent<Particle>()->SetParticleSize(60 - (_timeCountPlayerR * 75), 60 - (_timeCountPlayerR * 75));
	GameObject* playerREnd4 = sceneInstance.GetCurrentScene()->GetGameObject("PlayerREnd4");

	if (_timeCountPlayerR < 0.2f)
	{
		playerREnd2->GetComponent<Particle>()->SetParticleSize(_timeCountPlayerR * 150, _timeCountPlayerR * 150);
		playerREnd4->GetComponent<Particle>()->SetParticleSize(_timeCountPlayerR * 150, _timeCountPlayerR * 150);
	}
	else
	{
		playerREnd2->GetComponent<Particle>()->SetParticleSize(60 - (_timeCountPlayerR * 80), 60 - (_timeCountPlayerR * 80));
		playerREnd4->GetComponent<Particle>()->SetParticleSize(60 - (_timeCountPlayerR * 80), 60 - (_timeCountPlayerR * 80));
	}

	//if (_isBezierTeleportSetting == false)
	//{
	DirectX::XMFLOAT2 bezierTeleport;
	DirectX::XMFLOAT2 teleportPosition = { 0,0 };
	DirectX::XMFLOAT2 bezierTeleport1 = { 10.0f, 10.0f };  // 두 중간점 사이가 좁을 수록 가파른 곡선
	DirectX::XMFLOAT2 bezierTeleport2 = { 20.0f, 20.0f };
	DirectX::XMFLOAT2 bezierTeleportEnd = { 150.0f, 150.0f };

	bezierTeleport = BezierPoint2D(teleportPosition, bezierTeleport1, bezierTeleport2, bezierTeleportEnd, _timeCount);

	_particleTelepotyPointList.push_back(bezierTeleport);

	//	_isBezierTeleportSetting = true;
	//}

	bossTeleportAttack1->GetComponent<Particle>()->SetParticleSize(_particleTelepotyPointList[0].x, _particleTelepotyPointList[0].y);
	bossTeleportAttack2->GetComponent<Particle>()->SetParticleSize(_particleTelepotyPointList[0].x + 20, _particleTelepotyPointList[0].y + 20);
	bossTeleportAttack3->GetComponent<Particle>()->SetParticleSize(_particleTelepotyPointList[0].x + 20, _particleTelepotyPointList[0].y + 20);
	bossTeleportAttack4->GetComponent<Particle>()->SetParticleSize(_particleTelepotyPointList[0].x, _particleTelepotyPointList[0].y);


	//DirectX::XMFLOAT3 testEnd;
	//DirectX::XMFLOAT3 testEnd_2;
	//DirectX::XMFLOAT3 testEnd1; // 베지어로 폭발세팅 테스트중
	//DirectX::XMFLOAT3 testEnd2;
	//DirectX::XMFLOAT3 testEnd3;

	//testEnd = { 10,10,10 };
	//testEnd_2 = { 15,15,15 };
	//testEnd1 = { 20,20,20 };
	//testEnd2 = { 30,30,30 };
	//testEnd3 = { 70,70,70 };

	//particlePoint3 = Bezier(testEnd, testEnd1, testEnd2, testEnd3, _timeCount * 2   );
	//particlePoint3_2 = Bezier(testEnd_2, testEnd1, testEnd2, testEnd3, _timeCount * 2   );

	//PlayerE33->GetComponent<Particle>()->SetParticleSize(2 * particlePoint3_2.x, 2 * particlePoint3_2.y);
	//PlayerE2_2->GetComponent<Particle>()->SetParticleSize(1 * particlePoint3.x, 1 * particlePoint3.y);
//}
//else
//{
	for (auto& timeCount : _timeCountList)
	{
		timeCount += TimeManager::GetInstance().GetDeltaTime() / 4;
	}
	//}

	int timeCountIndex = 0;

	for (auto& timeCount : _timeCountList)
	{
		if (timeCount > 1.0f)
		{
			_bezierSwordSoulPointsList.erase(_bezierSwordSoulPointsList.begin() + timeCountIndex);
			timeCount = 0.0f;
			//_isSettingTimer = false;

			ParticleSwordSoulList[timeCountIndex]->GetComponent<Transform>()->SetPosition(sword->GetComponent<Transform>()->GetPosition().x + ToolBox::GetRandomFloat(-50.0f, 50.0f),
				sword->GetComponent<Transform>()->GetPosition().y + ToolBox::GetRandomFloat(-20.0f, 20.0f), sword->GetComponent<Transform>()->GetPosition().z + ToolBox::GetRandomFloat(-62.0f, 62.0f));

			_bezierSwordSoulPointsList.insert(_bezierSwordSoulPointsList.begin() + timeCountIndex, BezierSetting(ParticleSwordSoulList[timeCountIndex]));
		}
		++timeCountIndex;
	}

	if (_timeCount > 0.2f)
	{
		_timeCount = 0.0f;
	}
	if (_timeCountPlayerR > 0.8f)
	{
		_timeCountPlayerR = 0.0f;
	}
	//{
	//	_bezierPointsList.clear();
	//	
	//	for (auto bezierObject : _bezierObjectList)
	//	{
	//		bezierObject->GetComponent<Transform>()->SetPosition(ToolBox::GetRandomFloat(-50.0f, 50.0f), ToolBox::GetRandomFloat(-20.0f, 20.0f), ToolBox::GetRandomFloat(-62.0f, 62.0f));

	//		_bezierPointsList.push_back(BezierSetting(bezierObject));
	//	}

	//
	//}


	_particlePointList.clear(); // 파티클 포인트 초기화 반드시 해줘야함
	_particleSwordSoulPointList.clear();
	_particleTelepotyPointList.clear();
}



void KunrealEngine::EngineCore::MapParticleSetting()
{
	DirectX::XMFLOAT4 Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT4 Diffuse = { 0.3f, 1.0f, 0.1f, 1.0f };
	DirectX::XMFLOAT4 Specular = { 1.0f, 1.0f, 1.0f, 1.0f };

	mapParticle1 = sceneInstance.GetCurrentScene()->CreateObject("MapParticle1");
	mapParticle1->GetComponent<Transform>()->SetPosition(-113.83, 20.31, -136.35f);
	mapParticle1->AddComponent<Particle>();
	mapParticle1->GetComponent<Particle>()->SetParticleEffect("Flare5", "Resources/Textures/Particles/fx_Twister2.dds", 1000);
	mapParticle1->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.5f);
	mapParticle1->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	mapParticle1->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	mapParticle1->GetComponent<Particle>()->AddParticleColor(0.3f, 10.0f, 0.0f);
	mapParticle1->GetComponent<Particle>()->SetParticleDirection(0.0f, 20.0f, 0.0f);
	mapParticle1->GetComponent<Particle>()->SetParticleCameraApply(true);
	mapParticle1->AddComponent<Light>();
	mapParticle1->GetComponent<Light>()->CreatePointLight(Ambient, Diffuse, Specular, 300);
	mapParticle1->_autoAwake = true;

	mapParticle2 = sceneInstance.GetCurrentScene()->CreateObject("MapParticle2");
	mapParticle2->GetComponent<Transform>()->SetPosition(-38.6, 13.88, -140.8f);
	mapParticle2->AddComponent<Particle>();
	mapParticle2->GetComponent<Particle>()->SetParticleEffect("Flare5", "Resources/Textures/Particles/fx_Twister2.dds", 1000);
	mapParticle2->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.5f);
	mapParticle2->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	mapParticle2->GetComponent<Particle>()->SetParticleSize(8.f, 8.0f);
	mapParticle2->GetComponent<Particle>()->AddParticleColor(3.0f, 10.0f, 1.0f);
	mapParticle2->GetComponent<Particle>()->SetParticleDirection(0.0f, 40.0f, 0.0f);
	mapParticle2->GetComponent<Particle>()->SetParticleCameraApply(true);
	mapParticle2->AddComponent<Light>();
	mapParticle2->GetComponent<Light>()->CreatePointLight(Ambient, Diffuse, Specular, 300);
	mapParticle2->_autoAwake = true;

	mapParticle3 = sceneInstance.GetCurrentScene()->CreateObject("MapParticle3");
	mapParticle3->GetComponent<Transform>()->SetPosition(39.18f, 13.88, -140.8f);
	mapParticle3->AddComponent<Particle>();
	mapParticle3->GetComponent<Particle>()->SetParticleEffect("Flare5", "Resources/Textures/Particles/fx_Twister2.dds", 1000);
	mapParticle3->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.5f);
	mapParticle3->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	mapParticle3->GetComponent<Particle>()->SetParticleSize(8.f, 8.0f);
	mapParticle3->GetComponent<Particle>()->AddParticleColor(3.0f, 10.0f, 1.0f);
	mapParticle3->GetComponent<Particle>()->SetParticleDirection(0.0f, 40.0f, 0.0f);
	mapParticle3->GetComponent<Particle>()->SetParticleCameraApply(true);
	mapParticle3->AddComponent<Light>();
	mapParticle3->GetComponent<Light>()->CreatePointLight(Ambient, Diffuse, Specular, 300);
	mapParticle3->_autoAwake = true;

	mapParticle4 = sceneInstance.GetCurrentScene()->CreateObject("MapParticle4");
	mapParticle4->GetComponent<Transform>()->SetPosition(115.42, 20.31, -136.35f);
	mapParticle4->AddComponent<Particle>();
	mapParticle4->GetComponent<Particle>()->SetParticleEffect("Flare5", "Resources/Textures/Particles/fx_Twister2.dds", 1000);
	mapParticle4->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.5f);
	mapParticle4->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	mapParticle4->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	mapParticle4->GetComponent<Particle>()->AddParticleColor(0.3f, 10.0f, 0.0f);
	mapParticle4->GetComponent<Particle>()->SetParticleDirection(0.0f, 20.0f, 0.0f);
	mapParticle4->GetComponent<Particle>()->SetParticleCameraApply(true);
	mapParticle4->AddComponent<Light>();
	mapParticle4->GetComponent<Light>()->CreatePointLight(Ambient, Diffuse, Specular, 300);
	mapParticle4->_autoAwake = true;

	mapParticle5 = sceneInstance.GetCurrentScene()->CreateObject("MapParticle5");
	mapParticle5->GetComponent<Transform>()->SetPosition(115.42f, 21.316f, 124.14f);
	mapParticle5->AddComponent<Particle>();
	mapParticle5->GetComponent<Particle>()->SetParticleEffect("Flare5", "Resources/Textures/Particles/fx_Twister2.dds", 1000);
	mapParticle5->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.5f);
	mapParticle5->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	mapParticle5->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	mapParticle5->GetComponent<Particle>()->AddParticleColor(0.3f, 10.0f, 0.0f);
	mapParticle5->GetComponent<Particle>()->SetParticleDirection(0.0f, 20.0f, 0.0f);
	mapParticle5->GetComponent<Particle>()->SetParticleCameraApply(true);
	mapParticle5->AddComponent<Light>();
	mapParticle5->GetComponent<Light>()->CreatePointLight(Ambient, Diffuse, Specular, 300);
	mapParticle5->_autoAwake = true;

	mapParticle6 = sceneInstance.GetCurrentScene()->CreateObject("MapParticle6");
	mapParticle6->GetComponent<Transform>()->SetPosition(28.16f, 47.125f, 138.95f);
	mapParticle6->AddComponent<Particle>();
	mapParticle6->GetComponent<Particle>()->SetParticleEffect("Flare5", "Resources/Textures/Particles/fx_Twister2.dds", 1000);
	mapParticle6->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.6f);
	mapParticle6->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	mapParticle6->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	mapParticle6->GetComponent<Particle>()->AddParticleColor(1.0f, 10.0f, 0.0f);
	mapParticle6->GetComponent<Particle>()->SetParticleDirection(0.0f, 20.0f, 0.0f);
	mapParticle6->GetComponent<Particle>()->SetParticleCameraApply(true);
	mapParticle6->AddComponent<Light>();
	mapParticle6->GetComponent<Light>()->CreatePointLight(Ambient, Diffuse, Specular, 300);
	mapParticle6->_autoAwake = true;

	mapParticle7 = sceneInstance.GetCurrentScene()->CreateObject("MapParticle7");
	mapParticle7->GetComponent<Transform>()->SetPosition(-27.11f, 47.125f, 138.95f);
	mapParticle7->AddComponent<Particle>();
	mapParticle7->GetComponent<Particle>()->SetParticleEffect("Flare5", "Resources/Textures/Particles/fx_Twister2.dds", 1000);
	mapParticle7->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.6f);
	mapParticle7->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	mapParticle7->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	mapParticle7->GetComponent<Particle>()->AddParticleColor(1.0f, 10.0f, 0.0f);
	mapParticle7->GetComponent<Particle>()->SetParticleDirection(0.0f, 20.0f, 0.0f);
	mapParticle7->GetComponent<Particle>()->SetParticleCameraApply(true);
	mapParticle7->AddComponent<Light>();
	mapParticle7->GetComponent<Light>()->CreatePointLight(Ambient, Diffuse, Specular, 300);
	mapParticle7->_autoAwake = true;

	mapParticle8 = sceneInstance.GetCurrentScene()->CreateObject("MapParticle8");
	mapParticle8->GetComponent<Transform>()->SetPosition(-113.37f, 21.316f, 125.89f);
	mapParticle8->AddComponent<Particle>();
	mapParticle8->GetComponent<Particle>()->SetParticleEffect("Flare5", "Resources/Textures/Particles/fx_Twister2.dds", 1000);
	mapParticle8->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.5f);
	mapParticle8->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
	mapParticle8->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	mapParticle8->GetComponent<Particle>()->AddParticleColor(0.3f, 10.0f, 0.0f);
	mapParticle8->GetComponent<Particle>()->SetParticleDirection(0.0f, 20.0f, 0.0f);
	mapParticle8->GetComponent<Particle>()->SetParticleCameraApply(true);
	mapParticle8->AddComponent<Light>();
	mapParticle8->GetComponent<Light>()->CreatePointLight(Ambient, Diffuse, Specular, 300);
	mapParticle8->_autoAwake = true;

	mapParticleEye1 = sceneInstance.GetCurrentScene()->CreateObject("MapParticleEye1");
	mapParticleEye1->GetComponent<Transform>()->SetPosition(122.3f, 28.13f, -5.02f);
	mapParticleEye1->AddComponent<Particle>();
	mapParticleEye1->GetComponent<Particle>()->SetParticleEffect("Lightning1", "Resources/Textures/Particles/fx_Lightning1.dds", 1000);
	mapParticleEye1->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	mapParticleEye1->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
	mapParticleEye1->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	mapParticleEye1->GetComponent<Particle>()->AddParticleColor(1.0f, 0.1f, 0.0f);
	mapParticleEye1->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	mapParticleEye1->_autoAwake = true;

	mapParticleEye2 = sceneInstance.GetCurrentScene()->CreateObject("MapParticleEye2");
	mapParticleEye2->GetComponent<Transform>()->SetPosition(64.84f, 52.395f, 215.4f);
	mapParticleEye2->AddComponent<Particle>();
	mapParticleEye2->GetComponent<Particle>()->SetParticleEffect("Lightning1", "Resources/Textures/Particles/fx_Lightning1.dds", 1000);
	mapParticleEye2->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	mapParticleEye2->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
	mapParticleEye2->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	mapParticleEye2->GetComponent<Particle>()->AddParticleColor(1.0f, 0.1f, 0.0f);
	mapParticleEye2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	mapParticleEye2->_autoAwake = true;

	mapParticleEye3 = sceneInstance.GetCurrentScene()->CreateObject("MapParticleEye3");
	mapParticleEye3->GetComponent<Transform>()->SetPosition(42.37f, 29, 144.6f);
	mapParticleEye3->AddComponent<Particle>();
	mapParticleEye3->GetComponent<Particle>()->SetParticleEffect("Blaster1", "Resources/Textures/Particles/fx_Blaster1.dds", 1000);
	mapParticleEye3->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	mapParticleEye3->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
	mapParticleEye3->GetComponent<Particle>()->SetParticleSize(3.f, 1.0f);
	mapParticleEye3->GetComponent<Particle>()->AddParticleColor(10.0f, 0.1f, 0.0f);
	mapParticleEye3->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	mapParticleEye3->GetComponent<Particle>()->SetParticleAngle(0.0f, 0.0f, 356.0f);
	mapParticleEye3->_autoAwake = true;

	mapParticleEye4 = sceneInstance.GetCurrentScene()->CreateObject("MapParticleEye4");
	mapParticleEye4->GetComponent<Transform>()->SetPosition(37.45f, 29, 144.6f);
	mapParticleEye4->AddComponent<Particle>();
	mapParticleEye4->GetComponent<Particle>()->SetParticleEffect("Blaster1", "Resources/Textures/Particles/fx_Blaster1.dds", 1000);
	mapParticleEye4->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	mapParticleEye4->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
	mapParticleEye4->GetComponent<Particle>()->SetParticleSize(3.f, 1.0f);
	mapParticleEye4->GetComponent<Particle>()->AddParticleColor(10.0f, 0.4f, 0.0f);
	mapParticleEye4->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	mapParticleEye4->GetComponent<Particle>()->SetParticleAngle(0.0f, 0.0f, 30.0f);
	mapParticleEye4->_autoAwake = true;

	mapParticleEye5 = sceneInstance.GetCurrentScene()->CreateObject("MapParticleEye5");
	mapParticleEye5->GetComponent<Transform>()->SetPosition(-63.866f, 52.395f, 215.93f);
	mapParticleEye5->AddComponent<Particle>();
	mapParticleEye5->GetComponent<Particle>()->SetParticleEffect("Lightning1", "Resources/Textures/Particles/fx_Lightning1.dds", 1000);
	mapParticleEye5->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	mapParticleEye5->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
	mapParticleEye5->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	mapParticleEye5->GetComponent<Particle>()->AddParticleColor(1.0f, 0.1f, 0.0f);
	mapParticleEye5->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	mapParticleEye5->_autoAwake = true;

	mapParticleEye6 = sceneInstance.GetCurrentScene()->CreateObject("MapParticleEye6");
	mapParticleEye6->GetComponent<Transform>()->SetPosition(-36.433f, 29, 144.6f);
	mapParticleEye6->AddComponent<Particle>();
	mapParticleEye6->GetComponent<Particle>()->SetParticleEffect("Blaster1", "Resources/Textures/Particles/fx_Blaster1.dds", 1000);
	mapParticleEye6->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	mapParticleEye6->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
	mapParticleEye6->GetComponent<Particle>()->SetParticleSize(3.f, 1.0f);
	mapParticleEye6->GetComponent<Particle>()->AddParticleColor(10.0f, 0.1f, 0.0f);
	mapParticleEye6->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	mapParticleEye6->GetComponent<Particle>()->SetParticleAngle(0.0f, 0.0f, 356.0f);
	mapParticleEye6->_autoAwake = true;

	mapParticleEye7 = sceneInstance.GetCurrentScene()->CreateObject("MapParticleEye7");
	mapParticleEye7->GetComponent<Transform>()->SetPosition(-41.3f, 29, 144.6f);
	mapParticleEye7->AddComponent<Particle>();
	mapParticleEye7->GetComponent<Particle>()->SetParticleEffect("Blaster1", "Resources/Textures/Particles/fx_Blaster1.dds", 1000);
	mapParticleEye7->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	mapParticleEye7->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
	mapParticleEye7->GetComponent<Particle>()->SetParticleSize(3.f, 1.0f);
	mapParticleEye7->GetComponent<Particle>()->AddParticleColor(10.0f, 0.4f, 0.0f);
	mapParticleEye7->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	mapParticleEye7->GetComponent<Particle>()->SetParticleAngle(0.0f, 0.0f, 30.0f);
	mapParticleEye7->_autoAwake = true;

	mapParticleEye8 = sceneInstance.GetCurrentScene()->CreateObject("MapParticleEye8");
	mapParticleEye8->GetComponent<Transform>()->SetPosition(-120.72f, 28.02f, -5.02f);
	mapParticleEye8->AddComponent<Particle>();
	mapParticleEye8->GetComponent<Particle>()->SetParticleEffect("Lightning1", "Resources/Textures/Particles/fx_Lightning1.dds", 1000);
	mapParticleEye8->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	mapParticleEye8->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
	mapParticleEye8->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	mapParticleEye8->GetComponent<Particle>()->AddParticleColor(1.0f, 0.1f, 0.0f);
	mapParticleEye8->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	mapParticleEye8->_autoAwake = true;
}

void KunrealEngine::EngineCore::MoveToMain()
{
	if (sceneInstance.GetCurrentScene()->GetSceneName() == "Title")
	{
		if (sceneInstance.GetCurrentScene()->GetGameObject("BossPortal")->GetCollider()->IsCollided()
			&& sceneInstance.GetCurrentScene()->GetGameObject("BossPortal")->GetCollider()->GetTargetObject() == sceneInstance.GetCurrentScene()->GetObjectWithTag("Player"))
		{
			sceneInstance.ChangeScene("Main");
			sceneInstance.GetScene("Main")->GetGameObject("Player")->GetComponent<PlayerMove>()->SetPlayerY(2.0f);
			navigationInstance.HandleBuild(0, "bossmap.obj");
			navigationInstance.HandleBuild(1, "bossmap.obj");
			EventManager::GetInstance().SetCamera("testCamera");
		}

	}
}

void KunrealEngine::EngineCore::FloatingY(GameObject* name, float time)
{
	auto transform = name->GetComponent<Transform>();
	float objectX = name->GetComponent<Transform>()->GetPosition().x;
	float objectY = name->GetComponent<Transform>()->GetPosition().y;
	float objectZ = name->GetComponent<Transform>()->GetPosition().z;


	if (transform->_floatingFactor >= 0.0f && transform->_floatingFactor <= time)
	{
		transform->_floatingFactor += TimeManager::GetInstance().GetDeltaTime();
		name->GetComponent<Transform>()->SetPosition(objectX, objectY + 0.03f, objectZ);

		if (transform->_floatingFactor >= time - 1.0f || transform->_floatingFactor <= 1.0f)
		{
			name->GetComponent<Transform>()->SetPosition(objectX, objectY + 0.02f, objectZ);
		}

		if (transform->_floatingFactor >= time)
		{
			transform->_floatingHelper = 0.0f;
		}
	}
	else if (transform->_floatingHelper >= 0.0f && transform->_floatingHelper <= time)
	{
		transform->_floatingHelper += TimeManager::GetInstance().GetDeltaTime();
		name->GetComponent<Transform>()->SetPosition(objectX, objectY - 0.03f, objectZ);

		if (transform->_floatingHelper >= time - 1.0f || transform->_floatingHelper <= 1.0f)
		{
			name->GetComponent<Transform>()->SetPosition(objectX, objectY - 0.02f, objectZ);
		}

		if (transform->_floatingHelper >= time)
		{
			transform->_floatingFactor = 0.0f;
		}
	}
}

void KunrealEngine::EngineCore::ShiveringLight(GameObject* name)
{
	name->GetComponent<Light>()->SetOffSet(ToolBox::GetRandomFloat(-3.0f, 3.0f), 15.f, 0.0f);
}

void KunrealEngine::EngineCore::CreateTitleScene()
{
	Scene* titleScene = sceneInstance.CreateScene("Title");
	sceneInstance.ChangeScene("Title");

	navigationInstance.HandleBuild(0, "bridge_mapmesh.obj");

	// Camera
	GameObject* titleCamera = sceneInstance.GetCurrentScene()->CreateObject("TitleCamera");
	DirectX::XMFLOAT3 cameraPos = { 0.0f, 0.0f, 1.0f };

	DirectX::XMFLOAT3 targetPos = { 0.0f, 0.0f, 0.0f };
	titleCamera->AddComponent<Camera>();
	titleCamera->GetComponent<Camera>()->SetCameraPosition(cameraPos.x, cameraPos.y, cameraPos.z);
	titleCamera->GetComponent<Camera>()->SetTargetPosition(targetPos.x, targetPos.y, targetPos.z);

	titleCamera->GetComponent<Camera>()->SetMainCamera();

	titleCamera->GetComponent<Transform>()->SetPosition({ -155.0f, 100.0f,-148.130f });
	titleCamera->GetComponent<Transform>()->SetRotation(-180.f, 0.f, 0.f);

	EventManager::GetInstance().SetCamera("TitleCamera");

	// light test
	DirectX::XMFLOAT4 diffuse = { 0.3f, 0.3f, 0.3f, 0.3f };
	DirectX::XMFLOAT4 ambient = { 0.2f, 0.2f, 0.2f, 0.2f };
	DirectX::XMFLOAT4 specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 direction = { 1.0f, -1.0f, -1.0f };

	GameObject* titleLight = sceneInstance.GetCurrentScene()->CreateObject("DirectionalLight");
	titleLight->AddComponent<Light>();
	Light* light = titleLight->GetComponent<Light>();
	light->CreateDirectionalLight(ambient, diffuse, specular, direction);
	light->SetActive(true);
	titleLight->SetActive(true);

	GameObject* bridge = sceneInstance.GetCurrentScene()->CreateObject("Bridge");
	bridge->GetComponent<Transform>()->SetPosition(-155.0f, 66.0f, 0.0f);
	bridge->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	bridge->GetComponent<Transform>()->SetRotation(0.0f, 90.0f, 0.0f);
	bridge->AddComponent<MeshRenderer>();

	MeshRenderer* bridgeMesh = bridge->GetComponent<MeshRenderer>();

	bridgeMesh->SetMeshObject("Bridge/Bridge", true);

	//// cube map test
	GRAPHICS->CreateCubeMap("TitleBackground", "DarkMoon.dds", true);
	GRAPHICS->SetMainCubeMap("TitleBackground");

	GameObject* titleUIpack = MakeTitleUIPack();

	DirectX::XMFLOAT4 titleAmbient = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT4 titleDiffuse = { 0.0f, 1.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 titleSpecular = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 부유물 1
	titleRock1 = sceneInstance.GetCurrentScene()->CreateObject("TitleRock1");
	titleRock1->AddComponent<MeshRenderer>();
	titleRock1->GetComponent<MeshRenderer>()->SetMeshObject("FloatingLight1/FloatingLight1");
	titleRock1->GetComponent<Transform>()->SetPosition(-96.0f, 95.f, -30.f);
	titleRock1->GetComponent<Transform>()->SetScale(0.05f, 0.05f, 0.05f);
	titleRock1->AddComponent<Light>();
	titleRock1->GetComponent<Light>()->CreatePointLight(titleAmbient, titleDiffuse, titleSpecular, 300.f, 32.f);
	titleRock1->GetComponent<Light>()->SetOffSet(0.0f, 20.f, 0.0f);
	titleRock1->AddComponent<Particle>();
	titleRock1->GetComponent<Particle>()->SetParticleEffect("FloatingFire1", "Resources/Textures/Particles/flare.dds", 1000);
	titleRock1->GetComponent<Particle>()->SetOffSet(0.0f, 10.f, 0.0f);
	titleRock1->GetComponent<Particle>()->SetParticleSize(15.0f, 15.0f);
	titleRock1->GetComponent<Particle>()->SetParticleVelocity(8, true);
	titleRock1->GetComponent<Particle>()->SetParticleDuration(1.5f, 2.0f);
	titleRock1->GetComponent<Particle>()->SetParticleDirection(0.0f, 10.f, 0.0f);
	titleRock1->GetComponent<Particle>()->AddParticleColor(0.0f, 1.0f, 0.0f);
	titleRock1->GetComponent<Particle>()->SetActive(true);

	// 부유물 2
	titleRock2 = sceneInstance.GetCurrentScene()->CreateObject("TitleRock2");
	titleRock2->AddComponent<MeshRenderer>();
	titleRock2->GetComponent<MeshRenderer>()->SetMeshObject("FloatingLight2/FloatingLight2");
	titleRock2->GetComponent<Transform>()->SetPosition(-230.0f, 100.f, 47.f);
	titleRock2->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	titleRock2->AddComponent<Light>();
	titleRock2->GetComponent<Light>()->CreatePointLight(titleAmbient, titleDiffuse, titleSpecular, 300.f, 32.f);
	GameObject* titleRock2Par = sceneInstance.GetCurrentScene()->CreateObject("TitleRock2Particle");
	titleRock2Par->AddComponent<Particle>();
	titleRock2Par->GetComponent<Particle>()->SetParticleEffect("FloatingFire2", "Resources/Textures/Particles/flare.dds", 1000);
	//titleRock2Par->GetComponent<Particle>()->SetParticlePos(-230.0f, 110.f, 47.f);
	titleRock2Par->GetComponent<Transform>()->SetPosition(-230.0f, 115.f, 47.f);
	titleRock2Par->GetComponent<Particle>()->SetParticleSize(15.0f, 15.0f);
	titleRock2Par->GetComponent<Particle>()->SetParticleVelocity(8, true);
	titleRock2Par->GetComponent<Particle>()->SetParticleDuration(1.5f, 2.0f);
	titleRock2Par->GetComponent<Particle>()->AddParticleColor(0.0f, 1.0f, 0.0f);
	titleRock2Par->GetComponent<Particle>()->SetParticleDirection(0.0f, 10.f, 0.0f);
	titleRock2Par->GetComponent<Particle>()->SetActive(true);

	// 부유물 3
	titleRock3 = sceneInstance.GetCurrentScene()->CreateObject("TitleRock3");
	titleRock3->AddComponent<MeshRenderer>();
	titleRock3->GetComponent<MeshRenderer>()->SetMeshObject("FloatingLight3/FloatingLight3");
	titleRock3->GetComponent<Transform>()->SetScale(5.f, 5.f, 5.f);
	titleRock3->GetComponent<Transform>()->SetPosition(-98.2f, 92.f, 183.6f);
	titleRock3->GetComponent<Transform>()->SetRotation(180.f, 0.f, 0.f);
	titleRock3->AddComponent<Light>();
	titleRock3->GetComponent<Light>()->CreatePointLight(titleAmbient, titleDiffuse, titleSpecular, 300.f, 32.f);
	titleRock3->AddComponent<Particle>();
	titleRock3->GetComponent<Particle>()->SetParticleEffect("FloatingFire1", "Resources/Textures/Particles/flare.dds", 1000);
	titleRock3->GetComponent<Particle>()->SetOffSet(0.0f, 10.f, 0.0f);
	titleRock3->GetComponent<Particle>()->SetParticleSize(15.0f, 15.0f);
	titleRock3->GetComponent<Particle>()->SetParticleVelocity(8, true);
	titleRock3->GetComponent<Particle>()->SetParticleDuration(1.5f, 2.0f);
	titleRock3->GetComponent<Particle>()->SetParticleDirection(0.0f, 10.f, 0.0f);
	titleRock3->GetComponent<Particle>()->AddParticleColor(0.0f, 1.0f, 0.0f);
	titleRock3->GetComponent<Particle>()->SetActive(true);

	// 부유물 4
	floatingObj = sceneInstance.GetCurrentScene()->CreateObject("FloatingObj");
	floatingObj->AddComponent<MeshRenderer>();
	floatingObj->GetComponent<MeshRenderer>()->SetMeshObject("FloatingObj/FloatingObj");
	floatingObj->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	floatingObj->GetComponent<Transform>()->SetPosition(-28.f, 95.5f, 156.f);
	floatingObj->GetComponent<Transform>()->SetRotation(-63.f, 39.f, 17.f);

	GameObject* titlePlayer = sceneInstance.GetCurrentScene()->CreateObject("TitlePlayer");
	titlePlayer->AddComponent<Player>();
	titlePlayer->GetComponent<Transform>()->SetPosition(-156.0f, 66.f, 0.0f);
	titlePlayer->GetComponent<PlayerMove>()->SetPlayerY(66.0f);
	DirectX::XMFLOAT4 pdiffuse = { 0.3f, 0.3f, 0.3f, 0.3f };
	DirectX::XMFLOAT4 pambient = { 0.2f, 0.2f, 0.2f, 0.2f };
	DirectX::XMFLOAT4 pspecular = { 1.0f, 1.0f, 1.0f, 1.0f };
	titlePlayer->AddComponent<Light>();
	titlePlayer->GetComponent<Light>()->CreatePointLight(pambient, pdiffuse, pspecular, 300.f, 16.f);
	titlePlayer->GetComponent<Light>()->SetOffSet(0.0f, 15.0f, -5.f);

	GameObject* bossPortal = sceneInstance.GetCurrentScene()->CreateObject("BossPortal");
	bossPortal->GetComponent<Transform>()->SetPosition(-156.0f, 66.f, 220.0f);
	bossPortal->AddComponent<BoxCollider>();
	bossPortal->GetComponent<BoxCollider>()->SetColliderScale(30.0f, 70.0f, 30.0f);

	// 포탈 파티클

	particlePortal1 = sceneInstance.GetCurrentScene()->CreateObject("particlePortal1");
	particlePortal1->GetComponent<Transform>()->SetPosition(-159, 90, 240);
	particlePortal1->AddComponent<Particle>();
	particlePortal1->GetComponent<Particle>()->SetParticleEffect("LightFlash2", "Resources/Textures/Particles/fx_LightFlash2.dds", 1000);
	particlePortal1->GetComponent<Particle>()->SetParticleDuration(0.6f, 1.0f);
	particlePortal1->GetComponent<Particle>()->SetParticleVelocity(80.0f, true);
	particlePortal1->GetComponent<Particle>()->SetParticleSize(140.0f, 140.0f);
	particlePortal1->GetComponent<Particle>()->AddParticleColor(0.05f, 0.0f, 1.0f);
	particlePortal1->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particlePortal2 = sceneInstance.GetCurrentScene()->CreateObject("particlePortal2");
	particlePortal2->GetComponent<Transform>()->SetPosition(-159, 90, 180);
	particlePortal2->AddComponent<Particle>();
	particlePortal2->GetComponent<Particle>()->SetParticleEffect("BlastWave1", "Resources/Textures/Particles/fx_BlastWave1.dds", 1000);
	particlePortal2->GetComponent<Particle>()->SetParticleDuration(2.0f, 3.0f);
	particlePortal2->GetComponent<Particle>()->SetParticleVelocity(20.0f, true);
	particlePortal2->GetComponent<Particle>()->SetParticleSize(80.0f, 80.0f);
	particlePortal2->GetComponent<Particle>()->AddParticleColor(0.01f, 0.0f, 1.0f);
	particlePortal2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particlePortal3 = sceneInstance.GetCurrentScene()->CreateObject("particlePortal3");
	particlePortal3->GetComponent<Transform>()->SetPosition(-159, 90, 180);
	particlePortal3->AddComponent<Particle>();
	particlePortal3->GetComponent<Particle>()->SetParticleEffect("BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
	particlePortal3->GetComponent<Particle>()->SetParticleDuration(3.0f, 0.4f);
	particlePortal3->GetComponent<Particle>()->SetParticleVelocity(20.0f, true);
	particlePortal3->GetComponent<Particle>()->SetParticleSize(110.0f, 110.0f);
	particlePortal3->GetComponent<Particle>()->AddParticleColor(0.2f, 0.0f, 1.0f);
	particlePortal3->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	GameObject* particleMoon = sceneInstance.GetCurrentScene()->CreateObject("particleMoon");
	particleMoon->GetComponent<Transform>()->SetPosition(-152.121f, 182.672f, 180);
	particleMoon->AddComponent<Particle>();
	particleMoon->GetComponent<Particle>()->SetParticleEffect("BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
	particleMoon->GetComponent<Particle>()->SetParticleDuration(3.0f, 0.4f);
	particleMoon->GetComponent<Particle>()->SetParticleVelocity(9.0f, true);
	particleMoon->GetComponent<Particle>()->SetParticleSize(15.0f, 15.0f);
	particleMoon->GetComponent<Particle>()->AddParticleColor(0.2f, 1.0f, 0.0f);
	particleMoon->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
}