#pragma once
#include <windows.h>
#include "CommonHeader.h"
#include "KunrealAPI.h"

	extern KunrealEngine::GameObject* titleRock1;
	extern KunrealEngine::GameObject* titleRock2;
	extern KunrealEngine::GameObject* titleRock3;
	extern KunrealEngine::GameObject* floatingObj;
	extern KunrealEngine::GameObject* titleBoss;

namespace KunrealEngine
{

	static void MapParticleSetting(std::vector<GameObject*>& mapParticleList)
	{
		DirectX::XMFLOAT4 Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
		DirectX::XMFLOAT4 Diffuse = { 0.3f, 1.0f, 0.1f, 1.0f };
		DirectX::XMFLOAT4 Specular = { 1.0f, 1.0f, 1.0f, 1.0f };

		GameObject* mapParticle1 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MapParticle1");
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
		mapParticleList.emplace_back(mapParticle1);

		GameObject* mapParticle2 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MapParticle2");
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
		mapParticleList.emplace_back(mapParticle2);

		GameObject* mapParticle3 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MapParticle3");
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
		mapParticleList.emplace_back(mapParticle3);

		GameObject* mapParticle4 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MapParticle4");
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
		mapParticleList.emplace_back(mapParticle4);

		GameObject* mapParticle5 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MapParticle5");
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
		mapParticleList.emplace_back(mapParticle5);

		GameObject* mapParticle6 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MapParticle6");
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
		mapParticleList.emplace_back(mapParticle6);

		GameObject* mapParticle7 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MapParticle7");
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
		mapParticleList.emplace_back(mapParticle7);

		GameObject* mapParticle8 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MapParticle8");
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
		mapParticleList.emplace_back(mapParticle8);

		GameObject* mapParticleEye1 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MapParticleEye1");
		mapParticleEye1->GetComponent<Transform>()->SetPosition(122.3f, 28.13f, -5.02f);
		mapParticleEye1->AddComponent<Particle>();
		mapParticleEye1->GetComponent<Particle>()->SetParticleEffect("Lightning1", "Resources/Textures/Particles/fx_Lightning1.dds", 1000);
		mapParticleEye1->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
		mapParticleEye1->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
		mapParticleEye1->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
		mapParticleEye1->GetComponent<Particle>()->AddParticleColor(1.0f, 0.1f, 0.0f);
		mapParticleEye1->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
		mapParticleEye1->_autoAwake = true;
		mapParticleList.emplace_back(mapParticleEye1);

		GameObject* mapParticleEye2 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MapParticleEye2");
		mapParticleEye2->GetComponent<Transform>()->SetPosition(64.84f, 52.395f, 215.4f);
		mapParticleEye2->AddComponent<Particle>();
		mapParticleEye2->GetComponent<Particle>()->SetParticleEffect("Lightning1", "Resources/Textures/Particles/fx_Lightning1.dds", 1000);
		mapParticleEye2->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
		mapParticleEye2->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
		mapParticleEye2->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
		mapParticleEye2->GetComponent<Particle>()->AddParticleColor(1.0f, 0.1f, 0.0f);
		mapParticleEye2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
		mapParticleEye2->_autoAwake = true;
		mapParticleList.emplace_back(mapParticleEye2);

		GameObject* mapParticleEye3 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MapParticleEye3");
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
		mapParticleList.emplace_back(mapParticleEye3);

		GameObject* mapParticleEye4 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MapParticleEye4");
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
		mapParticleList.emplace_back(mapParticleEye4);

		GameObject* mapParticleEye5 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MapParticleEye5");
		mapParticleEye5->GetComponent<Transform>()->SetPosition(-63.866f, 52.395f, 215.93f);
		mapParticleEye5->AddComponent<Particle>();
		mapParticleEye5->GetComponent<Particle>()->SetParticleEffect("Lightning1", "Resources/Textures/Particles/fx_Lightning1.dds", 1000);
		mapParticleEye5->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
		mapParticleEye5->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
		mapParticleEye5->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
		mapParticleEye5->GetComponent<Particle>()->AddParticleColor(1.0f, 0.1f, 0.0f);
		mapParticleEye5->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
		mapParticleEye5->_autoAwake = true;
		mapParticleList.emplace_back(mapParticleEye5);

		GameObject* mapParticleEye6 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MapParticleEye6");
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
		mapParticleList.emplace_back(mapParticleEye6);

		GameObject* mapParticleEye7 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MapParticleEye7");
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
		mapParticleList.emplace_back(mapParticleEye7);

		GameObject* mapParticleEye8 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MapParticleEye8");
		mapParticleEye8->GetComponent<Transform>()->SetPosition(-120.72f, 28.02f, -5.02f);
		mapParticleEye8->AddComponent<Particle>();
		mapParticleEye8->GetComponent<Particle>()->SetParticleEffect("Lightning1", "Resources/Textures/Particles/fx_Lightning1.dds", 1000);
		mapParticleEye8->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
		mapParticleEye8->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
		mapParticleEye8->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
		mapParticleEye8->GetComponent<Particle>()->AddParticleColor(1.0f, 0.1f, 0.0f);
		mapParticleEye8->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
		mapParticleEye8->_autoAwake = true;
		mapParticleList.emplace_back(mapParticleEye8);

	}
	
	void TitlesceneobjectSetting(std::vector<GameObject*>& bezierObjectList)
	{
		auto& sceneInstance = SceneManager::GetInstance();

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

		GameObject* particlePortal1 = sceneInstance.GetCurrentScene()->CreateObject("particlePortal1");
		particlePortal1->GetComponent<Transform>()->SetPosition(-159, 90, 240);
		particlePortal1->AddComponent<Particle>();
		particlePortal1->GetComponent<Particle>()->SetParticleEffect("LightFlash2", "Resources/Textures/Particles/fx_LightFlash2.dds", 1000);
		particlePortal1->GetComponent<Particle>()->SetParticleDuration(0.6f, 1.0f);
		particlePortal1->GetComponent<Particle>()->SetParticleVelocity(80.0f, true);
		particlePortal1->GetComponent<Particle>()->SetParticleSize(140.0f, 140.0f);
		particlePortal1->GetComponent<Particle>()->AddParticleColor(0.05f, 0.0f, 1.0f);
		particlePortal1->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

		GameObject* particlePortal2 = sceneInstance.GetCurrentScene()->CreateObject("particlePortal2");
		particlePortal2->GetComponent<Transform>()->SetPosition(-159, 90, 180);
		particlePortal2->AddComponent<Particle>();
		particlePortal2->GetComponent<Particle>()->SetParticleEffect("BlastWave1", "Resources/Textures/Particles/fx_BlastWave1.dds", 1000);
		particlePortal2->GetComponent<Particle>()->SetParticleDuration(2.0f, 3.0f);
		particlePortal2->GetComponent<Particle>()->SetParticleVelocity(20.0f, true);
		particlePortal2->GetComponent<Particle>()->SetParticleSize(80.0f, 80.0f);
		particlePortal2->GetComponent<Particle>()->AddParticleColor(0.01f, 0.0f, 1.0f);
		particlePortal2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

		GameObject* particlePortal3 = sceneInstance.GetCurrentScene()->CreateObject("particlePortal3");
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

		titleBoss = sceneInstance.GetCurrentScene()->CreateObject("titleBoss");
		titleBoss->AddComponent<MeshRenderer>();
		titleBoss->GetComponent<MeshRenderer>()->SetMeshObject("Lich/Lich");
		titleBoss->GetComponent<Transform>()->SetScale(42.6f, 30.3f, 30.3f);
		titleBoss->GetComponent<Transform>()->SetPosition(-159.f, -173.f, 751.f);
		titleBoss->GetComponent<Transform>()->SetRotation(5.f, -10.f, -1.f);
		auto texSize = titleBoss->GetComponent<MeshRenderer>()->GetTextures().size();
		for (int i = 0; i < texSize; i++)
		{
			titleBoss->GetComponent<MeshRenderer>()->SetDiffuseTexture(i, "Lich/T_Lich_1_D.tga");
			titleBoss->GetComponent<MeshRenderer>()->SetNormalTexture(i, "Lich/T_Lich_N.tga");
			titleBoss->GetComponent<MeshRenderer>()->SetEmissiveTexture(i, "Lich/T_Lich_01_E.tga");
		}
		//DirectX::XMFLOAT4 bdiffuse = { 0.3f, 0.3f, 0.3f, 0.3f };
		//DirectX::XMFLOAT4 bambient = { 0.2f, 0.2f, 0.2f, 0.2f };
		//DirectX::XMFLOAT4 bspecular = { 1.0f, 1.0f, 1.0f, 1.0f };
		//titleBoss->AddComponent<Light>();
		//titleBoss->GetComponent<Light>()->CreatePointLight(bambient, bdiffuse,bspecular, 500, 64.f);
		//titleBoss->GetComponent<Light>()->SetOffSet(0.0f, 60.f, 0.0f);

		for (int i = 0; i < 10; ++i)
		{
			GameObject* particleBezierTest;
			particleBezierTest = sceneInstance.GetCurrentScene()->CreateObject("Particle18");
			particleBezierTest->GetComponent<Transform>()->SetPosition(-152.121f + ToolBox::GetRandomFloat(-150.0f, 150.0f), 182.672f + ToolBox::GetRandomFloat(-100.0f, 100.0f), 180);
			particleBezierTest->AddComponent<Particle>();
			particleBezierTest->GetComponent<Particle>()->SetParticleEffect("Dust1", "Resources/Textures/Particles/fx_Dust1.dds", 1000);
			particleBezierTest->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.0f);
			particleBezierTest->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
			particleBezierTest->GetComponent<Particle>()->SetParticleSize(5.0f, 5.0f);
			particleBezierTest->GetComponent<Particle>()->AddParticleColor(1.0f, 5.0f, 0.0f);
			particleBezierTest->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);

			bezierObjectList.push_back(particleBezierTest);
		}
	}
}