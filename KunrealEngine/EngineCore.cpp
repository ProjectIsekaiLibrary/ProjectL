#include "EngineCore.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "GraphicsSystem.h"
#include "PhysicsSystem.h"
#include "InputSystem.h"
#include "SoundSystem.h"

#include "ComponentHeaders.h"

/// 꼭 지워야해 내 수학라이브러리와 동일한 결과가 나오는지 체크용
#include <DirectXMath.h>
/// 

KunrealEngine::SceneManager& sceneInstance = KunrealEngine::SceneManager::GetInstance();
KunrealEngine::TimeManager& timeInstance = KunrealEngine::TimeManager::GetInstance();
KunrealEngine::InputSystem* inputInstance = KunrealEngine::InputSystem::GetInstance();
KunrealEngine::SoundSystem& soundInstance = KunrealEngine::SoundSystem::GetInstance();

GInterface::GraphicsRenderable* testObject1;
GInterface::GraphicsRenderable* testObject2;
GInterface::GraphicsRenderable* testObject3;
GInterface::GraphicsRenderable* testObject4;
GInterface::GraphicsRenderable* testObject5;


//GInterface::GraphicsCamera* testCamera;

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
	soundInstance.AddSound("Resources/Sound/sound01.wav", "Title", 0);

	// camera component test
	auto ptr = sceneInstance.GetCurrentScene()->CreateObject("testCamera");

	KunrealEngine::KunrealMath::Float3 a = { 0.0f, 5.0f, -30.0f };
	KunrealEngine::KunrealMath::Float3 b = { 0.0f, 0.0f, 0.0f };

	ptr->AddComponent<KunrealEngine::Camera>();
	ptr->GetComponent<KunrealEngine::Camera>()->SetCameraPosition(a.x, a.y, a.z);
	ptr->GetComponent<KunrealEngine::Camera>()->SetTargetPosition(b.x, b.y, b.z);

	ptr->GetComponent<KunrealEngine::Camera>()->SetMainCamera();
	ptr->GetComponent<KunrealEngine::Camera>()->RotateCamera(0.f, 0.f);

	// transform component test
	KunrealEngine::Transform* transtrans = ptr->GetComponent<Transform>();

	// mesh renderer component test
	auto pigTest = sceneInstance.GetCurrentScene()->CreateObject("pigTest");
	pigTest->AddComponent<MeshRenderer>();
	pigTest->GetComponent<MeshRenderer>()->SetMeshObject("Resources/ASEFile/babypig_idle_6x.ASE");
	pigTest->GetComponent<Transform>()->SetPosition(-20, 0, 0);
	pigTest->GetComponent<Transform>()->SetScale(0.05f, 0.05f, 0.05f);
	pigTest->GetComponent<MeshRenderer>()->SetActive(false);
	KunrealEngine::KunrealMath::Float4 reflect = { 0.9f, 0.9f, 0.9f, 0.9f };
	sceneInstance.GetCurrentScene()->GetGameObject("pigTest")->GetComponent<MeshRenderer>()->SetDiffuseTexture("Resources/Textures/RainbowTree_Texture01.dds");
	sceneInstance.GetCurrentScene()->GetGameObject("pigTest")->GetComponent<MeshRenderer>()->SetReflect(reflect.x, reflect.y, reflect.z, reflect.w);

	// ase static test
	testObject1 = GRAPHICS->CreateRenderable("Resources/ASEFile/genji_max.ASE", "Resources/Textures/000000002405.dds");
	testObject1->SetPosition(0.0f, -100.0f, 500.0f);
	testObject1->SetScale(100.0f, 100.0f, 100.0f);
	testObject1->SetRenderingState(false);

	// ase animation test
	testObject2 = GRAPHICS->CreateRenderable("Resources/ASEFile/babypig_idle_6x.ASE", "Resources/Textures/RainbowTree_Texture01.dds");
	testObject2->SetPosition(20.0f);
	testObject2->SetScale(0.05f, 0.05f, 0.05f);
	testObject2->SetRenderingState(false);

	// fbx test
	testObject3 = GRAPHICS->CreateRenderable("Kachujin/Kachujin", "Resources/Textures/Kachujin/Kachujin_diffuse.png");
	testObject3->SetPosition(0.0f);
	testObject3->SetScale(0.05f, 0.05f, 0.05f);
	testObject3->SetNormalTexture("Resources/Textures/Kachujin/Kachujin_normal.png");
	testObject3->SetRenderingState(true);

	// cube map test
	GRAPHICS->CreateCubeMap("test", "Resources/Textures/CubeMaps/mipmapJaeHak.dds");
	auto list = GRAPHICS->GetCubeMapList();
	GRAPHICS->SetMainCubeMap(list.back());

	// light test
	KunrealEngine::KunrealMath::Float4 ambient = { 0.01f, 0.01f, 0.01f, 0.01f };
	KunrealEngine::KunrealMath::Float4 diffuse = { 0.01f, 0.01f, 0.01f, 0.01f };
	KunrealEngine::KunrealMath::Float4 specular = { 0.01f, 0.01f, 0.01f, 0.01f };
	KunrealEngine::KunrealMath::Float3 direction = { 1.f, 0.f, 0.0f };

	KunrealEngine::KunrealMath::Float4 ambient2 = { 1.0f, 1.0f, 1.0f, 1.0f };
	KunrealEngine::KunrealMath::Float4 diffuse2 = { 1.0f, 1.0f, 1.0f, 1.0f };
	KunrealEngine::KunrealMath::Float4 specular2 = { 1.0f, 1.0f, 1.0f, 1.0f };
	KunrealEngine::KunrealMath::Float3 direction2 = { 0.707f, 0.707f, 0.0f };


	auto lightTest = sceneInstance.GetCurrentScene()->CreateObject("lightTest");
	lightTest->AddComponent<Light>();
	lightTest->GetComponent<Light>()->CreateDirectionalLight(ambient2, diffuse2, specular2, direction);
	lightTest->GetComponent<Light>()->SetAmbient(1.f, 1.f, 1.f, 1.f); 
	//lightTest->GetComponent<Light>()->SetActive(false);
	lightTest->SetActive(false);

	// PointLight 예시
	KunrealEngine::KunrealMath::Float3 tempPosition = { 0.0f, 10.0f, 0.0f };
	//GInterface::GraphicsPointLight* pointLight = GRAPHICS->CreatePointLight(ambient2, diffuse2, specular2, tempPosition, 3.0f);
	//pointLight->SetActive(true);

	auto pointLight = sceneInstance.GetCurrentScene()->CreateObject("pointLightTest");
	pointLight->AddComponent<Light>();
	pointLight->GetComponent<Transform>()->SetPosition(0.f, 10.f, 0.f);
	pointLight->GetComponent<Light>()->CreatePointLight(ambient2, diffuse2, specular2, 3.0f);
	pointLight->GetComponent<Light>()->SetDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	pointLight->SetActive(false);

	/// 로직 테스트
	KunrealMath::Matrix4x4 _m1(1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4);
	KunrealMath::Matrix4x4 _m2(1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4);
	KunrealMath::Matrix4x4 _m3(1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4);

	KunrealMath::Matrix4x4 _m4 = KunrealMath::Multiply4x4Matrix(_m1, _m2, _m3);
	std::string testName = sceneInstance.GetCurrentScene()->GetGameObject("pigTest")->GetComponent<MeshRenderer>()->GetComponentName();
	std::string testName1 = sceneInstance.GetCurrentScene()->GetGameObject("pigTest")->GetComponent<Transform>()->GetComponentName();
	std::string testName2 = ptr->GetComponent<KunrealEngine::Camera>()->GetComponentName();

	int aqwe = 10;
}

void KunrealEngine::EngineCore::Finalize()
{
	sceneInstance.Finalize();
	timeInstance.Finalize();
	KunrealEngine::GraphicsSystem::GetInstance().Finalize();
	/// 나머지들 finalize 할 것들 추가
}

void KunrealEngine::EngineCore::FixedUpdate()
{
	sceneInstance.FixedUpdateScene(sceneInstance.GetCurrentScene());
}

void KunrealEngine::EngineCore::Update()
{
	sceneInstance.UpdateScene(sceneInstance.GetCurrentScene());
	//inputInstance->Update(GetDeltaTime());
	
	soundInstance.Loop(soundInstance.FindIndexOfSound("Title"));

	testObject2->PlayAnimation(1000.0f * TimeManager::GetInstance().GetDeltaTime(), true);
	//testObject3->PlayAnimation(1000.0f * TimeManager::GetInstance().GetDeltaTime(), true);

	GRAPHICS->DrawDebugText(100, 100, "FPS : %.2f", 1 / TimeManager::GetInstance().GetDeltaTime());
	GRAPHICS->DrawDebugText(100, 300, "%s", sceneInstance.GetCurrentScene()->GetSceneName().c_str());

	// 카메라 테스트

// 	if (inputInstance->KeyUp(KEY::A))
// 	{
// 		GRAPHICS->GetMainCamera()->Strafe(-10.0f * TimeManager::GetInstance().GetDeltaTime());
// 	}

	if (GetAsyncKeyState('A'))
	{
		GRAPHICS->GetMainCamera()->Strafe(-10.0f * TimeManager::GetInstance().GetDeltaTime());
	}
	else if (GetAsyncKeyState('D'))
	{
		GRAPHICS->GetMainCamera()->Strafe(10.0f * TimeManager::GetInstance().GetDeltaTime());
	}
	else if (GetAsyncKeyState('W'))
	{
		GRAPHICS->GetMainCamera()->Walk(10.0f * TimeManager::GetInstance().GetDeltaTime());
	}
	else if (GetAsyncKeyState('S'))
	{
		GRAPHICS->GetMainCamera()->Walk(-10.0f * TimeManager::GetInstance().GetDeltaTime());
	}
	else if (GetAsyncKeyState('Q'))
	{
		GRAPHICS->GetMainCamera()->UpDown(-10.0f * TimeManager::GetInstance().GetDeltaTime());
	}
	else if (GetAsyncKeyState('E'))
	{
		GRAPHICS->GetMainCamera()->UpDown(10.0f * TimeManager::GetInstance().GetDeltaTime());
	}
	else if (GetAsyncKeyState('R'))
	{
		GRAPHICS->GetMainCamera()->RotateCamera({0,  -20.0f * TimeManager::GetInstance().GetDeltaTime()});
	}
	else if (GetAsyncKeyState('T'))
	{
		GRAPHICS->GetMainCamera()->RotateCamera({ 0,  20.0f * TimeManager::GetInstance().GetDeltaTime() });
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

void* KunrealEngine::EngineCore::GetRenderingImage()
{
	return GRAPHICS->GetRenderingImage();
}

void* KunrealEngine::EngineCore::GetDevice()
{
	return GRAPHICS->GetDevice();
}

void* KunrealEngine::EngineCore::GetDeviceContext()
{
	return GRAPHICS->GetDeviceContext();
}

void* KunrealEngine::EngineCore::GetRenderTargetView()
{
	return GRAPHICS->GetRenderTargetView();
}

// KunrealEngine::SceneManager& KunrealEngine::EngineCore::GetSceneManager()
// {
// 	return sceneInstance;
// }

float KunrealEngine::EngineCore::GetDeltaTime()
{
	return timeInstance.GetDeltaTime();
}
