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
#include "EventManager.h"

#include "ComponentHeaders.h"

#include "UIPackageFuntion.hpp"

/// 꼭 지워야해 내 수학라이브러리와 동일한 결과가 나오는지 체크용
#include <DirectXMath.h>
#include "ToolBox.h"
/// 

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
KunrealEngine::GameObject* tree1;
KunrealEngine::GameObject* tree2;
KunrealEngine::GameObject* tree3;
KunrealEngine::GameObject* tree4;

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

KunrealEngine::GameObject* testCamera;



DirectX::XMFLOAT3 targetPos;

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
	sceneInstance.GetCurrentScene()->DeleteGameObject(meshTest);

	PhysicsSystem::GetInstance().Initialize();
	inputInstance->Initialize(hInstance, hwnd, screenHeight, screenWidth);
	soundInstance.Initialize(hwnd);

	navigationInstance.Initialize();
	//navigationInstance.LoadAll("Resources/Navimesh/0_p.bin", 0);
	navigationInstance.HandleBuild(0, "testObj");
	navigationInstance.HandleBuild(1, "testObj");

	//// cube map test
	GRAPHICS->CreateCubeMap("test", "Texture6.dds", true);
	auto list = GRAPHICS->GetCubeMapList();
	GRAPHICS->SetMainCubeMap(list.back());

	sceneInstance.CreateScene("mapTest2.json");
	sceneInstance.CreateScene("mapTest3.json");
	sceneInstance.CreateScene("mapTest4.json");
	sceneInstance.CreateScene("mapTest6.json");

	sceneInstance.CreateScene("ParticleTest");

	ChangeScene("ParticleTest");
	ParticleTest();
	/// 니들 맘대로 해
	//PlayGround();
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
	//particleBossSword3->GetComponent<Particle>()->SetParticleSize(5.f * ToolBox::GetRandomFloat(0.3f, 1.0f), 5.0f * ToolBox::GetRandomFloat(0.1f, 1.0f));
	CheckMousePosition();
	inputInstance->Update(GetDeltaTime());
	sceneInstance.UpdateScene(sceneInstance.GetCurrentScene());

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
	
	navigationInstance.HandleUpdate(TimeManager::GetInstance().GetDeltaTime());

	//std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3> pos = kamen->GetComponent<Kamen>()->GetBossPosition();
	//navigationInstance.MoveTempObstacle(pos.first, pos.second);
	
	// 장애물 설치 테스트
	if (inputInstance->KeyInput(KEY::LSHIFT) && inputInstance->MouseButtonDown(0))
	{
		//RemoveObstacle();
	}
	else if (inputInstance->MouseButtonDown(0))
	{
		//MakeObstacle();
	}


	if (inputInstance->KeyUp(KEY::CAPSLOCK))
	{
		//sceneInstance.GetCurrentScene()->GetGameObject("BaeSung")->AddComponent<BoxCollider>();
		GRAPHICS->DrawUIText(100, 100, 20, DirectX::XMFLOAT4(255.0f, 0.0f, 255.0f, 255.0f), "FPS : %.2f", 1 / TimeManager::GetInstance().GetDeltaTime());
	}
	//GRAPHICS->DrawDebugText(100, 100, 20, "FPS : %.2f", 1 / TimeManager::GetInstance().GetDeltaTime());

	inputInstance->GetMousePosition(_ingameMouseX, _ingameMouseY);

	//cursorimage->SetPosition(_ingameMouseX, _ingameMouseY);
	//cursorimage->SetScale(0.9 * 0.1, 1.6 * 0.1);

	//spider->GetComponent<Animator>()->Play("Idle", 70.f, true);

	auto gameObjectList = GetCurrentScene()->GetObjectList();


	if (GetCurrentScene()->GetGameObject("RuneStoneGolem") != nullptr)
	{
		if (GetCurrentScene()->GetGameObject("RuneStoneGolem")->GetComponent<BoxCollider>()->IsCollided() == true)
		{
			if (GetCurrentScene()->GetGameObject("RuneStoneGolem")->GetComponent<BoxCollider>()->GetTargetObject() == GetCurrentScene()->GetGameObject("Player") && InputSystem::GetInstance()->KeyUp(KEY::G))
			{
				ChangeScene("mapTest2.json");
			}
		}
	}


	if (GetCurrentScene()->GetGameObject("RuneStoneEnte") != nullptr)
	{
		if (GetCurrentScene()->GetGameObject("RuneStoneEnte")->GetComponent<BoxCollider>()->IsCollided() == true)
		{
			if (GetCurrentScene()->GetGameObject("RuneStoneEnte")->GetComponent<BoxCollider>()->GetTargetObject() == GetCurrentScene()->GetGameObject("Player") && InputSystem::GetInstance()->KeyUp(KEY::G))
			{
				KunrealEngine::GetCurrentScene()->GetGameObject("Player")->GetComponent<Transform>()->SetPosition(-52, 6, -72);
				KunrealEngine::GetCurrentScene()->GetGameObject("Player")->GetComponent<PlayerMove>()->SetPlayerY(6.0);
				//KunrealEngine::GetCurrentScene()->GetGameObject("Player")->MoveToScene("mapTest3.json");
				KunrealEngine::GetCurrentScene()->GetGameObject("Player")->GetComponent<Player>()->MoveToScene("mapTest3.json");
				ChangeScene("mapTest3.json");
				navigationInstance.LoadAll("Resources/Navimesh/3-p.bin", 0);

				navigationInstance.LoadAll("Resources/Navimesh/3-p.bin", 1);

				std::vector<DirectX::XMFLOAT3> vertices;
				std::vector<unsigned int> indices;
				navigationInstance.GetNavmeshRenderInfo(1, vertices, indices);
				GRAPHICS->CreateMapDebug("navimesh4_4", vertices, indices);

			}
		}
	}

		if (GetCurrentScene()->GetGameObject("RuneStoneKamen") != nullptr)
	{
		if (GetCurrentScene()->GetGameObject("RuneStoneKamen")->GetComponent<BoxCollider>()->IsCollided() == true)
		{
			if (GetCurrentScene()->GetGameObject("RuneStoneKamen")->GetComponent<BoxCollider>()->GetTargetObject() == GetCurrentScene()->GetGameObject("Player") && InputSystem::GetInstance()->KeyUp(KEY::G))
			{
				KunrealEngine::GetCurrentScene()->GetGameObject("Player")->GetComponent<Transform>()->SetPosition(-32, 2.2, -72);
				KunrealEngine::GetCurrentScene()->GetGameObject("Player")->GetComponent<PlayerMove>()->SetPlayerY(2.2f);
				//KunrealEngine::GetCurrentScene()->GetGameObject("Player")->MoveToScene("mapTest4.json");
				KunrealEngine::GetCurrentScene()->GetGameObject("Player")->GetComponent<Player>()->MoveToScene("mapTest4.json");
				ChangeScene("mapTest4.json");

				navigationInstance.LoadAll("Resources/Navimesh/4-p.bin", 0);

				navigationInstance.LoadAll("Resources/Navimesh/4-b.bin", 1);

				std::vector<DirectX::XMFLOAT3> vertices;
				std::vector<unsigned int> indices;
				navigationInstance.GetNavmeshRenderInfo(0, vertices, indices);
				GRAPHICS->CreateMapDebug("navimesh4_4", vertices, indices);

			}
		}
	}

	if (GetCurrentScene()->GetGameObject("RuneStoneArachne") != nullptr)
	{
		if (GetCurrentScene()->GetGameObject("RuneStoneArachne")->GetComponent<BoxCollider>()->IsCollided() == true)
		{
			if (GetCurrentScene()->GetGameObject("RuneStoneArachne")->GetComponent<BoxCollider>()->GetTargetObject() == GetCurrentScene()->GetGameObject("Player") && InputSystem::GetInstance()->KeyUp(KEY::G))
			{
				ChangeScene("mapTest6.json");
			}
		}
	}

	if (InputSystem::GetInstance()->KeyUp(KEY::CAPSLOCK))
	{
		KunrealEngine::GetCurrentScene()->GetGameObject("Player")->GetComponent<Transform>()->SetPosition(0, 4, 0);
		//KunrealEngine::GetCurrentScene()->GetGameObject("Player")->MoveToScene("Main");
		KunrealEngine::GetCurrentScene()->GetGameObject("Player")->GetComponent<Player>()->MoveToScene("Main");
		ChangeScene("Main");
		navigationInstance.LoadAll("Resources/Navimesh/0-p.bin", 0);
	}
	Updatecoroutine();
}

void KunrealEngine::EngineCore::LateUpdate()
{
	sceneInstance.LateUpdateScene(sceneInstance.GetCurrentScene());
	GRAPHICS->Update();	// 그래픽스쪽 카메라 업데이트 관련
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
	
	testCamera->GetComponent<Transform>()->SetPosition(-32.f, 45.f, -32.f);
	testCamera->GetComponent<Transform>()->SetRotation(0.f, 45.f, 0.f);

	// Plane 
	auto plane = sceneInstance.GetCurrentScene()->CreateObject("plane");
	plane->AddComponent<MeshRenderer>();
	plane->GetComponent<MeshRenderer>()->SetMeshObject("cube/cube", true);
	plane->GetComponent<MeshRenderer>()->SetDiffuseTexture(0, "floor.dds");
	//plane->GetComponent<MeshRenderer>()->SetNormalTexture(0, "floor_nmap.dds");
	plane->GetComponent<Transform>()->SetScale(100.0f, 1.0f, 100.0f);
	plane->GetComponent<Transform>()->SetPosition(0, -1.0f, 0);
	plane->GetComponent<MeshRenderer>()->SetShadowState(false);
	plane->GetComponent<MeshRenderer>()->SetIsDissolve(true);
	_timeMan -= 0.01f;
	if (_timeMan <= 0.0f)
	{
		_timeMan = 1.0f;
	}
	plane->GetComponent<MeshRenderer>()->SetDissolve(0.5f);


	// Player
	player = sceneInstance.GetCurrentScene()->CreateObject("Player");
	//player->AddComponent<MeshRenderer>();
	//player->GetComponent<MeshRenderer>()->SetMeshObject("PlayerWithCloak/PlayerWithCloak");
	//player->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	//player->GetComponent<Transform>()->SetRotation(0.0f, 45.f, 0.0f);
	//player->GetComponent<MeshRenderer>()->SetActive(true);
	//player->GetComponent<MeshRenderer>()->SetPickableState(true);
	//player->AddComponent<BoxCollider>();
	//player->GetComponent<BoxCollider>()->SetBoxSize(3.0f, 8.0f, 5.0f);


	player->AddComponent<Player>();

	//kamen = sceneInstance.GetCurrentScene()->CreateObject("kamen");
	//kamen->AddComponent<Kamen>();
	//kamen->DeleteComponent(kamen->GetComponent<Kamen>());
	
// 	spider = sceneInstance.GetCurrentScene() -> CreateObject("spider");
// 	spider->AddComponent<Aracne>();

	// UI의 부모가 될 0,0pos객체
	battle_ui_box = MakeBattleUIPack();
	pause_ui_box = MakeMenuUIPack();
	option_ui_box = MakeOptionUIPack();
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

	cursorimage = GRAPHICS->CreateImage("floor.dds");
	cursorimage->SetPosition(500.0f);
	cursorimage->SetRenderingState(false);

	// 파티클 testCode
	//auto asdf2 = GRAPHICS->CreateParticle("fire", "Resources/Textures/Particles/fx_EnergyBolt9.dds", 1000);
	//asdf2->SetParticleColor(DirectX::XMFLOAT3{ 0.0f, 1.0f, 1.0f });
	//asdf2->SetEmitVelocity(3.0f, true);
	//asdf2->SetParticleTime(1.0f, 1.0f);
	//asdf2->SetParticleSize(DirectX::XMFLOAT2{ 10.f, 10.f });
	//asdf2->SetEmitPos(DirectX::XMFLOAT3{ 0.0f, 30.0f, 0.0f });
	//asdf2->SetParticleDirection(DirectX::XMFLOAT3{ 0.0f, 30.0f, 0.0f });
	//asdf2->Start();
	//
	//auto asdf3 = GRAPHICS->CreateParticle("fire2", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
	//asdf3->SetParticleColor(DirectX::XMFLOAT3{ 1.0f, 1.0f, 0.0f });
	//asdf3->SetEmitVelocity(4.0f, true);
	//asdf3->SetParticleTime(1.0f, 3.0f);
	//asdf3->SetParticleSize(DirectX::XMFLOAT2{ 5.f, 5.f });
	//asdf3->SetEmitPos(DirectX::XMFLOAT3{ 0.0f, 28.0f, 0.0f });
	//asdf3->SetParticleDirection(DirectX::XMFLOAT3{ 0.0f, 7.0f, 0.0f });
	//asdf3->Start();

	GRAPHICS->CreateDebugLine(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT3(100.0f, 1.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
	//GRAPHICS->DeleteAllLine();
	// Test

	tree1 = sceneInstance.GetCurrentScene()->CreateObject("tree1");
	tree2 = sceneInstance.GetCurrentScene()->CreateObject("tree2");
	tree3 = sceneInstance.GetCurrentScene()->CreateObject("tree3");
	tree4 = sceneInstance.GetCurrentScene()->CreateObject("tree4");
	tree1->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	tree2->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);

	//for (int i = 0; i < 50; i++)
	//{
	//	std::string name = "rightCube" + std::to_string(i);
	//	auto cube1 = sceneInstance.GetCurrentScene()->CreateObject(name);
	//	cube1->AddComponent<MeshRenderer>();
	//	cube1->GetComponent<MeshRenderer>()->SetMeshObject("SM_tree_large_ancient_01/SM_tree_large_ancient_01", true);
	//	//cube1->GetComponent<MeshRenderer>()->SetDiffuseTexture(0, "bricks.dds");
	//	//cube1->GetComponent<MeshRenderer>()->SetNormalTexture(0, "bricks_nmap.dds");
	//	cube1->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	//	cube1->GetComponent<Transform>()->SetPosition(40.0f, 2.0f, 50.0f - 10.0f * i);
	//	//cube1->SetParent(tree1);
	//	if (i % 2 == 0)
	//	{
	//		cube1->GetComponent<MeshRenderer>()->SetPickableState(true);
	//	}
	//}
	//
	//for (int i = 0; i < 50; i++)
	//{
	//	std::string name = "leftCube" + std::to_string(i);
	//	auto cube1 = sceneInstance.GetCurrentScene()->CreateObject(name);
	//	cube1->AddComponent<MeshRenderer>();
	//	cube1->GetComponent<MeshRenderer>()->SetMeshObject("SM_tree_large_ancient_01/SM_tree_large_ancient_01", true);
	//	//cube1->GetComponent<MeshRenderer>()->SetDiffuseTexture(0, "bricks.dds");
	//	//cube1->GetComponent<MeshRenderer>()->SetNormalTexture("bricks_nmap.dds");
	//	cube1->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	//	cube1->GetComponent<Transform>()->SetPosition(-40.0f, 2.0f, 50.0f - 10.0f * i);
	//	//cube1->SetParent(tree2);
	//	if (i % 2 == 1)
	//	{
	//		cube1->GetComponent<MeshRenderer>()->SetPickableState(true);
	//	}
	//}
	//
	//for (int i = 0; i < 50; i++)
	//{
	//	std::string name = "leftCubee" + std::to_string(i);
	//	auto cube1 = sceneInstance.GetCurrentScene()->CreateObject(name);
	//	cube1->AddComponent<MeshRenderer>();
	//	cube1->GetComponent<MeshRenderer>()->SetMeshObject("SM_tree_large_ancient_02/SM_tree_large_ancient_02", true);
	//	//cube1->GetComponent<MeshRenderer>()->SetDiffuseTexture(0, "bricks.dds");
	//	//cube1->GetComponent<MeshRenderer>()->SetNormalTexture("bricks_nmap.dds");
	//	cube1->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	//	cube1->GetComponent<Transform>()->SetPosition(80.0f, 2.0f, 50.0f - 10.0f * i);
	//	//cube1->SetParent(tree3);
	//	if (i % 2 == 1)
	//	{
	//		cube1->GetComponent<MeshRenderer>()->SetPickableState(true);
	//	}
	//}
	//
	//for (int i = 0; i < 50; i++)
	//{
	//	std::string name = "rightCubee" + std::to_string(i);
	//	auto cube1 = sceneInstance.GetCurrentScene()->CreateObject(name);
	//	cube1->AddComponent<MeshRenderer>();
	//	cube1->GetComponent<MeshRenderer>()->SetMeshObject("SM_tree_large_ancient_02/SM_tree_large_ancient_02", true);
	//	//cube1->GetComponent<MeshRenderer>()->SetDiffuseTexture(0, "bricks.dds");
	//	//cube1->GetComponent<MeshRenderer>()->SetNormalTexture("bricks_nmap.dds");
	//	cube1->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	//	cube1->GetComponent<Transform>()->SetPosition(-80.0f, 2.0f, 50.0f - 10.0f * i);
	//	//cube1->SetParent(tree4);
	//	if (i % 2 == 1)
	//	{
	//		cube1->GetComponent<MeshRenderer>()->SetPickableState(true);
	//	}
	//}

	
	GameObject* particleTest = sceneInstance.GetCurrentScene()->CreateObject("ParticleTest");
	player->AddComponent<Particle>();

	Particle* partpart = player->GetComponent<Particle>();

	partpart->SetParticleEffect("fl", "Resources/Textures/Particles/fx_Thrust2.dds", 1000);
	partpart->SetParticleSize(4.0f, 4.0f);
	partpart->SetParticleVelocity(30.0f, true);
	partpart->SetParticleDuration(1.0f, 3.0f);
	partpart->AddParticleColor(1.0f, 0.0f, 0.0f);
	partpart->SetParticleDirection(0.0f, 20.0f, 0.0f);
	partpart->SetActive(true);

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
}

void KunrealEngine::EngineCore::MakeObstacle()
{
	DirectX::XMFLOAT3 targetPos = GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y);
	DirectX::XMFLOAT3 bmin = {4, 4, 4 };
	DirectX::XMFLOAT3 bmax = {4, 4, 4 };
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
	testCamera->GetComponent<Transform>()->SetPosition(-32.f, 45.f, 32.f);
	testCamera->GetComponent<Transform>()->SetRotation(0.f, 45.f, 60.f);

	// Plane 
	auto plane = sceneInstance.GetCurrentScene()->CreateObject("plane");
	plane->AddComponent<MeshRenderer>();
	plane->GetComponent<MeshRenderer>()->SetMeshObject("cube/cube", true);
	plane->GetComponent<MeshRenderer>()->SetDiffuseTexture(0, "floor.dds");
	//plane->GetComponent<MeshRenderer>()->SetNormalTexture(0, "floor_nmap.dds");
	plane->GetComponent<Transform>()->SetScale(100.0f, 1.0f, 100.0f);
	plane->GetComponent<Transform>()->SetPosition(0, -7.0f, 0);
	plane->GetComponent<MeshRenderer>()->SetShadowState(false);
	plane->GetComponent<MeshRenderer>()->SetIsDissolve(true);
	_timeMan -= 0.01f;
	if (_timeMan <= 0.0f)
	{
		_timeMan = 1.0f;
	}
	plane->GetComponent<MeshRenderer>()->SetDissolve(0.5f);

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

	particle = sceneInstance.GetCurrentScene()->CreateObject("Particle");
	particle->GetComponent<Transform>()->SetPosition(-20, 0, 0);
	particle->AddComponent<Particle>();
	particle->GetComponent<Particle>()->SetParticleEffect("Halo1","Resources/Textures/Particles/fx_Halo1.dds", 1000);
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
	particle9->GetComponent<Particle>()->SetParticleEffect("Lightning7", "Resources/Textures/Particles/fx_Lightning7.dds", 1000);
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
	particle11->GetComponent<Transform>()->SetPosition(20, 0, -25.f);
	particle11->AddComponent<Particle>();
	particle11->GetComponent<Particle>()->SetParticleEffect("Sparks1", "Resources/Textures/Particles/fx_Sparks1.dds", 1000);
	particle11->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle11->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	particle11->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle11->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	particle11->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 200.0f);

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
	particle15->GetComponent<Particle>()->SetParticleEffect("BlastWave2", "Resources/Textures/Particles/fx_BlastWave2.dds", 1000);
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
	particle16->GetComponent<Particle>()->SetParticleVelocity(25.0f, true);
	particle16->GetComponent<Particle>()->SetParticleSize(10.f, 10.0f);
	particle16->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 2.0f);
	particle16->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 200.0f);

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
	particle20->GetComponent<Particle>()->SetParticleEffect("Dust3", "Resources/Textures/Particles/fx_Dust3.dds", 10);
	particle20->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle20->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particle20->GetComponent<Particle>()->SetParticleSize(5.0f, 5.0f);
	particle20->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 2.0f);
	particle20->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particle21 = sceneInstance.GetCurrentScene()->CreateObject("Particle21");
	particle21->GetComponent<Transform>()->SetPosition(0, 0, 70.f);
	particle21->AddComponent<Particle>();
	particle21->GetComponent<Particle>()->SetParticleEffect("Dust4", "Resources/Textures/Particles/fx_Dust4.dds", 10);
	particle21->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle21->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particle21->GetComponent<Particle>()->SetParticleSize(5.0f, 5.0f);
	particle21->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 2.0f);
	particle21->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particle22 = sceneInstance.GetCurrentScene()->CreateObject("Particle22");
	particle22->GetComponent<Transform>()->SetPosition(10, 0, 70.f);
	particle22->AddComponent<Particle>();
	particle22->GetComponent<Particle>()->SetParticleEffect("EnergyBolt9", "Resources/Textures/Particles/fx_EnergyBolt9.dds", 1000);
	particle22->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle22->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particle22->GetComponent<Particle>()->SetParticleSize(5.0f, 5.0f);
	particle22->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 2.0f);
	particle22->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particle23 = sceneInstance.GetCurrentScene()->CreateObject("Particle23");
	particle23->GetComponent<Transform>()->SetPosition(20, 0, 70.f);
	particle23->AddComponent<Particle>();
	particle23->GetComponent<Particle>()->SetParticleEffect("EnergyBolt4", "Resources/Textures/Particles/fx_EnergyBolt4.dds", 1000);
	particle23->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle23->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particle23->GetComponent<Particle>()->SetParticleSize(5.0f, 5.0f);
	particle23->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 2.0f);
	particle23->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particle24 = sceneInstance.GetCurrentScene()->CreateObject("Particle24");
	particle24->GetComponent<Transform>()->SetPosition(30, 0, 70.f);
	particle24->AddComponent<Particle>();
	particle24->GetComponent<Particle>()->SetParticleEffect("EnergyBolt6", "Resources/Textures/Particles/fx_EnergyBolt6.dds", 1000);
	particle24->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle24->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particle24->GetComponent<Particle>()->SetParticleSize(5.0f, 5.0f);
	particle24->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 2.0f);
	particle24->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particle25 = sceneInstance.GetCurrentScene()->CreateObject("Particle25");
	particle25->GetComponent<Transform>()->SetPosition(-10, 0, 105.f);
	particle25->AddComponent<Particle>();
	particle25->GetComponent<Particle>()->SetParticleEffect("EnergyBolt7", "Resources/Textures/Particles/fx_EnergyBolt7.dds", 1000);
	particle25->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle25->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particle25->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle25->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	particle25->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particle26 = sceneInstance.GetCurrentScene()->CreateObject("Particle26");
	particle26->GetComponent<Transform>()->SetPosition(0, 0, 105.f);
	particle26->AddComponent<Particle>();
	particle26->GetComponent<Particle>()->SetParticleEffect("Fire1", "Resources/Textures/Particles/fx_Fire1.dds", 1000);
	particle26->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle26->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particle26->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle26->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	particle26->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particle27 = sceneInstance.GetCurrentScene()->CreateObject("Particle27");
	particle27->GetComponent<Transform>()->SetPosition(10, 0, 105.f);
	particle27->AddComponent<Particle>();
	particle27->GetComponent<Particle>()->SetParticleEffect("Fire3", "Resources/Textures/Particles/fx_Fire3.dds", 1000);
	particle27->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle27->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particle27->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle27->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	particle27->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particle28 = sceneInstance.GetCurrentScene()->CreateObject("Particle28");
	particle28->GetComponent<Transform>()->SetPosition(20, 0, 105.f);
	particle28->AddComponent<Particle>();
	particle28->GetComponent<Particle>()->SetParticleEffect("Fireball1", "Resources/Textures/Particles/fx_Fireball1.dds", 1000);
	particle28->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle28->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particle28->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle28->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	particle28->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particle29 = sceneInstance.GetCurrentScene()->CreateObject("Particle29");
	particle29->GetComponent<Transform>()->SetPosition(30, 0, 105.f);
	particle29->AddComponent<Particle>();
	particle29->GetComponent<Particle>()->SetParticleEffect("Flare5", "Resources/Textures/Particles/fx_Flare5.dds", 1000);
	particle29->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.5f);
	particle29->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	particle29->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particle29->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	particle29->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particle30 = sceneInstance.GetCurrentScene()->CreateObject("Particle30");
	particle30->GetComponent<Transform>()->SetPosition(-10, 0, 135.f);
	particle30->AddComponent<Particle>();
	particle30->GetComponent<Particle>()->SetParticleEffect("Flare6", "Resources / Textures / Particles / fx_Flare6.dds", 1000);
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
	particlePlayerW1->GetComponent<Particle>()->SetParticleEffect("Blast3", "Resources/Textures/Particles/fx_Blast3.dds", 10);
	particlePlayerW1->GetComponent<Particle>()->SetParticleDuration(4.0f, 6.0f);
	particlePlayerW1->GetComponent<Particle>()->SetParticleVelocity(4.0f, true);
	particlePlayerW1->GetComponent<Particle>()->SetParticleSize(3.f, 3.0f);
	particlePlayerW1->GetComponent<Particle>()->AddParticleColor(0.5f, 3.0f, 5.0f);
	particlePlayerW1->GetComponent<Particle>()->SetParticleDirection(0.0f, -1.0f, 0.0f);

	particlePlayerW2 = sceneInstance.GetCurrentScene()->CreateObject("PlayerW2");
	particlePlayerW2->GetComponent<Transform>()->SetPosition(-50, 0, 0.f);
	particlePlayerW2->AddComponent<Particle>();
	particlePlayerW2->GetComponent<Particle>()->SetParticleEffect("Dust3", "Resources/Textures/Particles/fx_Dust3.dds", 10);
	particlePlayerW2->GetComponent<Particle>()->SetParticleDuration(4.0f, 6.0f);
	particlePlayerW2->GetComponent<Particle>()->SetParticleVelocity(2.0f, true);
	particlePlayerW2->GetComponent<Particle>()->SetParticleSize(5.0f, 5.0f);
	particlePlayerW2->GetComponent<Particle>()->AddParticleColor(0.5f, 3.0f, 5.0f);
	particlePlayerW2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

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

	GameObject* arch = sceneInstance.GetCurrentScene()->CreateObject("arch");
	arch->GetComponent<Transform>()->SetPosition(68.8f, -2.4f, 48.7f);
	arch->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	arch->GetComponent<Transform>()->SetRotation(0, 59, 0);
	arch->AddComponent<MeshRenderer>();
	arch->GetComponent<MeshRenderer>()->SetMeshObject("SM_arch_large_ruins/SM_arch_large_ruins");

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
	particleBossCall1->GetComponent<Transform>()->SetPosition(-70, -2.2, 70);
	particleBossCall1->AddComponent<Particle>();
	particleBossCall1->GetComponent<Particle>()->SetParticleEffect("fx_Lightning4", "Resources/Textures/Particles/fx_Lightning4.dds", 1000);
	particleBossCall1->GetComponent<Particle>()->SetParticleDuration(3.9f, 0.9f);
	particleBossCall1->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	particleBossCall1->GetComponent<Particle>()->SetParticleSize(9.f, 3.0f);
	particleBossCall1->GetComponent<Particle>()->AddParticleColor(2.0f, 8.6f, 2.0f);
	particleBossCall1->GetComponent<Particle>()->SetParticleDirection(0.0f, 50.0f, 0.0f);

	particleBossCall2 = sceneInstance.GetCurrentScene()->CreateObject("particleBossCall2");
	particleBossCall2->GetComponent<Transform>()->SetPosition(-70, 14.5f, 70.f);
	particleBossCall2->AddComponent<Particle>();
	particleBossCall2->GetComponent<Particle>()->SetParticleEffect("Blaster2", "Resources/Textures/Particles/fx_Blaster2.dds", 1000);
	particleBossCall2->GetComponent<Particle>()->SetParticleDuration(0.9f, 0.15f);
	particleBossCall2->GetComponent<Particle>()->SetParticleVelocity(45.0f, true);
	particleBossCall2->GetComponent<Particle>()->SetParticleSize(4.f, 20.f);
	particleBossCall2->GetComponent<Particle>()->AddParticleColor(0.2f, 1.0f, 0.0f);
	particleBossCall2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

	particleBossCall3 = sceneInstance.GetCurrentScene()->CreateObject("particleBossCall3");
	particleBossCall3->GetComponent<Transform>()->SetPosition(-70, 0, 70.f);
	particleBossCall3->AddComponent<Particle>();
	particleBossCall3->GetComponent<Particle>()->SetParticleEffect("Fire1", "Resources/Textures/Particles/fx_Fire1.dds", 1000);
	particleBossCall3->GetComponent<Particle>()->SetParticleDuration(2.7f, 0.6f);
	particleBossCall3->GetComponent<Particle>()->SetParticleVelocity(12.4f, true);
	particleBossCall3->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	particleBossCall3->GetComponent<Particle>()->AddParticleColor(0.7f, 3.7f, 0.3f);
	particleBossCall3->GetComponent<Particle>()->SetParticleDirection(0.0f, 100.0f, 0.0f);


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

	sword->GetComponent<Transform>()->SetPosition(-84.3, 0, -35.f);
	sword->GetComponent<Transform>()->SetRotation(0, 135, 0);

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

	GameObject* sword2 = sceneInstance.GetCurrentScene()->CreateObject("sword");
	sword2->AddComponent<MeshRenderer>();
	sword2->GetComponent<MeshRenderer>()->SetMeshObject("KamenSword/KamenSword");
	sword2->GetComponent<MeshRenderer>()->SetDiffuseTexture(0, "KamenSword/KamenSword_BaseColor.png");
	sword2->GetComponent<MeshRenderer>()->SetNormalTexture(0, "KamenSword/KamenSword_Normal.png");
	sword2->GetComponent<MeshRenderer>()->SetEmissiveTexture(0, "KamenSword/KamenSword_Emissive.png");

	sword2->GetComponent<Transform>()->SetPosition(-84.3, 0, -55.f);
	sword2->GetComponent<Transform>()->SetRotation(0, 135, 0);

	GameObject* sword3 = sceneInstance.GetCurrentScene()->CreateObject("sword");
	sword3->AddComponent<MeshRenderer>();
	sword3->GetComponent<MeshRenderer>()->SetMeshObject("KamenSword/KamenSword");
	sword3->GetComponent<MeshRenderer>()->SetDiffuseTexture(0, "KamenSword/KamenSword_BaseColor.png");
	sword3->GetComponent<MeshRenderer>()->SetNormalTexture(0, "KamenSword/KamenSword_Normal.png");
	sword3->GetComponent<MeshRenderer>()->SetEmissiveTexture(0, "KamenSword/KamenSword_Emissive.png");

	sword3->GetComponent<Transform>()->SetPosition(-84.3, 0, -75.f);
	sword3->GetComponent<Transform>()->SetRotation(0, 135, 0);
}

float KunrealEngine::EngineCore::GetDeltaTime()
{
	return timeInstance.GetDeltaTime();
}
