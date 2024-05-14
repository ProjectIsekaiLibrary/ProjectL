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
KunrealEngine::GameObject* spider;
KunrealEngine::GameObject* zeolight;
KunrealEngine::GameObject* zeolight_Image;
KunrealEngine::GameObject* Button_Image1;
KunrealEngine::GameObject* Button_Image2;
KunrealEngine::GameObject* Button_Image3;
KunrealEngine::GameObject* Button_Image4;
KunrealEngine::GameObject* tree1;
KunrealEngine::GameObject* tree2;
KunrealEngine::GameObject* tree3;
KunrealEngine::GameObject* tree4;
KunrealEngine::GameObject* ent;
KunrealEngine::GameObject* ent2;


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

	GameObject* meshTest = sceneInstance.GetCurrentScene()->CreateObject("meshTest");
	meshTest->AddComponent<MeshRenderer>();
	meshTest->GetComponent<MeshRenderer>()->SetMeshObject("cylinder/cylinder");
	sceneInstance.GetCurrentScene()->DeleteGameObject(meshTest);

	PhysicsSystem::GetInstance().Initialize();
	inputInstance->Initialize(hInstance, hwnd, screenHeight, screenWidth);
	soundInstance.Initialize(hwnd);

	navigationInstance.Initialize();
	navigationInstance.HandleBuild(0);
	//navigationInstance.LoadAll("Resources/Navimesh/Player_navmesh.bin", 0);
	navigationInstance.HandleBuild(1);
	//navigationInstance.LoadAll("Resources/Navimesh/Boss_navmesh.bin", 1);
	//navigationInstance.LoadAll("Resources/Navimesh/Boss_navmesh.bin", 2);
	//navigationInstance.LoadAll("Resources/Navimesh/Boss_navmesh.bin", 3);
	//navigationInstance.LoadAll("Resources/Navimesh/Boss_navmesh.bin", 4);
	navigationInstance.SetSEpos(0, -23.0f, 0.0f, -10.0f, -90.0f, 0.0f, 96.0f);
	navigationInstance.SetSEpos(1, -23.0f, 0.0f, -10.0f, -90.0f, 0.0f, 96.0f);

	navigationInstance.AddTempObstacle(DirectX::XMFLOAT3(20.0f, 0.0f, 10.0f), 4.0f, 4.0f);

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

	//// cube map test
	GRAPHICS->CreateCubeMap("test", "sunsetcube1024.dds", true);
	auto list = GRAPHICS->GetCubeMapList();
	GRAPHICS->SetMainCubeMap(list.back());

	sceneInstance.CreateScene("mapTest2.json");
	sceneInstance.CreateScene("mapTest3.json");
	sceneInstance.CreateScene("mapTest4.json");
	sceneInstance.CreateScene("mapTest5.json");

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

	CheckMousePosition();

	if (inputInstance->KeyUp(KEY::CAPSLOCK))
	{
		//sceneInstance.GetCurrentScene()->GetGameObject("BaeSung")->AddComponent<BoxCollider>();
	}
	GRAPHICS->DrawDebugText(100, 100, 20, "FPS : %.2f", 1 / TimeManager::GetInstance().GetDeltaTime());

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
				ChangeScene("mapTest3.json");
			}
		}
	}

		if (GetCurrentScene()->GetGameObject("RuneStoneKamen") != nullptr)
	{
		if (GetCurrentScene()->GetGameObject("RuneStoneKamen")->GetComponent<BoxCollider>()->IsCollided() == true)
		{
			if (GetCurrentScene()->GetGameObject("RuneStoneKamen")->GetComponent<BoxCollider>()->GetTargetObject() == GetCurrentScene()->GetGameObject("Player") && InputSystem::GetInstance()->KeyUp(KEY::G))
			{
				ChangeScene("mapTest4.json");
			}
		}
	}

	if (GetCurrentScene()->GetGameObject("RuneStoneArachne") != nullptr)
	{
		if (GetCurrentScene()->GetGameObject("RuneStoneArachne")->GetComponent<BoxCollider>()->IsCollided() == true)
		{
			if (GetCurrentScene()->GetGameObject("RuneStoneArachne")->GetComponent<BoxCollider>()->GetTargetObject() == GetCurrentScene()->GetGameObject("Player") && InputSystem::GetInstance()->KeyUp(KEY::G))
			{
				ChangeScene("mapTest5.json");
			}
		}
	}

	if (InputSystem::GetInstance()->KeyUp(KEY::CAPSLOCK))
	{
		ChangeScene("Main");
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

	kamen = sceneInstance.GetCurrentScene()->CreateObject("kamen");
	kamen->AddComponent<Kamen>();
	//kamen->DeleteComponent(kamen->GetComponent<Kamen>());
	
	spider = sceneInstance.GetCurrentScene() -> CreateObject("spider");
	//spider -> AddComponent<MeshRenderer>();
	//spider -> GetComponent<MeshRenderer>() -> SetMeshObject("SpiderQueen/SpiderQueen", true);
	//spider->GetComponent<Transform>()->SetScale(10.0f, 10.0f, 10.0f);
	spider->AddComponent<Aracne>();

	ent = sceneInstance.GetCurrentScene()->CreateObject("ent");

	//ent -> AddComponent<Ent>();
	//ent -> GetComponent<MeshRenderer>() -> SetMeshObject("Ent_Generic/Ent_Generic", true);


	//ent2 = sceneInstance.GetCurrentScene()->CreateObject("ent2");
	//
	//ent2->AddComponent<MeshRenderer>();
	//ent2->GetComponent<MeshRenderer>()->SetMeshObject("Ent_Humanoid/Ent_Humanoid", true);
	//ent->GetComponent<Transform>()->SetScale(10.0f, 10.0f, 10.0f);
	//ent->GetComponent<Transform>()->SetScale(10.0f, 10.0f, 10.0f);

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
	//auto asdf2 = GRAPHICS->CreateParticle("fire", "Resources/Textures/Particles/flare.dds", 1000);
	//asdf2->SetParticleColor(DirectX::XMFLOAT3{1.0f, 0.0f, 0.0f});
	//asdf2->SetEmitVelocity(4.0f, true);
	//asdf2->SetParticleTime(1.0f, 3.0f);
	//asdf2->SetParticleSize(DirectX::XMFLOAT2{10.f, 10.f});
	//asdf2->SetEmitPos(DirectX::XMFLOAT3{ 0.0f, 30.0f, 0.0f });
	//asdf2->SetParticleDirection(DirectX::XMFLOAT3{0.0f, 7.0f, 0.0f});
	//asdf2->Start();
	//
	//auto asdf3 = GRAPHICS->CreateParticle("fire2", "Resources/Textures/Particles/flare.dds", 1000);
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

	
	//GameObject* particleTest = sceneInstance.GetCurrentScene()->CreateObject("ParticleTest");
	//player->AddComponent<Particle>();
	//
	//Particle* partpart = player->GetComponent<Particle>();
	//
	//partpart->SetParticleEffect("Fire", "Resources/Textures/Particles/flare.dds", 1000);
	//partpart->SetParticleSize(40.0f, 40.0f);
	//partpart->SetParticleVelocity(30.0f, true);
	//partpart->SetParticleDuration(1.0f, 3.0f);
	//partpart->AddParticleColor(1.0f, 0.0f, 0.0f);
	//partpart->SetParticleDirection(0.0f, 20.0f, 0.0f);
	//partpart->SetActive(true);	

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

float KunrealEngine::EngineCore::GetDeltaTime()
{
	return timeInstance.GetDeltaTime();
}
