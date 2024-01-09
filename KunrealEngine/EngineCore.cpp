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


	// mesh renderer component test
	auto modelTest = sceneInstance.GetCurrentScene()->CreateObject("modelTest");
	modelTest->AddComponent<MeshRenderer>();
	modelTest->GetComponent<MeshRenderer>()->SetMeshObject("Kachujin/Kachujin");
	modelTest->GetComponent<Transform>()->SetPosition(-20, 0, 0);
	modelTest->GetComponent<Transform>()->SetScale(0.05f, 0.05f, 0.05f);
	modelTest->GetComponent<MeshRenderer>()->SetActive(true);
	KunrealEngine::KunrealMath::Float4 reflect = { 0.9f, 0.9f, 0.9f, 0.9f };
	sceneInstance.GetCurrentScene()->GetGameObject("modelTest")->GetComponent<MeshRenderer>()->SetDiffuseTexture("Resources/Textures/Kachujin/Kachujin_diffuse.png");
	sceneInstance.GetCurrentScene()->GetGameObject("modelTest")->GetComponent<MeshRenderer>()->SetReflect(reflect.x, reflect.y, reflect.z, reflect.w);

	// Plane
	auto plane = sceneInstance.GetCurrentScene()->CreateObject("plane");
	plane->AddComponent<MeshRenderer>();
	plane->AddComponent<MeshRenderer>();
	plane->GetComponent<MeshRenderer>()->SetMeshObject("plane/plane", "Resources/Textures/bricks.dds", false);
	plane->GetComponent<MeshRenderer>()->SetDiffuseTexture("asdf");
	plane->GetComponent<Transform>()->SetRotation(90.0f, 0 ,0);
	plane->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);

	// cube map test
	GRAPHICS->CreateCubeMap("test", "Resources/Textures/CubeMaps/GodJaeHak.dds", true);
	auto list = GRAPHICS->GetCubeMapList();
	GRAPHICS->SetMainCubeMap(list.back());

	// light test
	KunrealEngine::KunrealMath::Float4 ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	KunrealEngine::KunrealMath::Float4 diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	KunrealEngine::KunrealMath::Float4 specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	KunrealEngine::KunrealMath::Float3 direction = { 0.707f, 0.707f, 0.0f };


	auto lightTest = sceneInstance.GetCurrentScene()->CreateObject("lightTest");
	lightTest->AddComponent<Light>();
	lightTest->GetComponent<Light>()->CreateDirectionalLight(ambient, diffuse, specular, direction);
	lightTest->GetComponent<Light>()->SetAmbient(1.f, 1.f, 1.f, 1.f); 
	lightTest->SetActive(false);

	// PointLight 예시
	auto pointLight = sceneInstance.GetCurrentScene()->CreateObject("pointLightTest");
	pointLight->AddComponent<Light>();
	pointLight->GetComponent<Transform>()->SetPosition(0.f, 0.f, 0.f);
	pointLight->GetComponent<Light>()->CreatePointLight(ambient, diffuse, specular, 1.0f);
	pointLight->GetComponent<Light>()->SetDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	pointLight->GetComponent<Light>()->SetPointRange(10.f);
	pointLight->GetComponent<Light>()->SetPointRange(100.0f);
	pointLight->SetActive(true);
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
	PhysicsSystem::GetInstance().FixedUpdate();
	sceneInstance.FixedUpdateScene(sceneInstance.GetCurrentScene());
}

void KunrealEngine::EngineCore::Update()
{
	sceneInstance.UpdateScene(sceneInstance.GetCurrentScene());
	//inputInstance->Update(GetDeltaTime());
	
	soundInstance.Loop(soundInstance.FindIndexOfSound("Title"));

	//testObject3->PlayAnimation(1000.0f * TimeManager::GetInstance().GetDeltaTime(), false);

	GRAPHICS->DrawDebugText(100, 100, "FPS : %.2f", 1 / TimeManager::GetInstance().GetDeltaTime());
	GRAPHICS->DrawDebugText(100, 300, "%s", sceneInstance.GetCurrentScene()->GetSceneName().c_str());

	// 카메라 테스트

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
