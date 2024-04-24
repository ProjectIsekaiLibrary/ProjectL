#include "Deserialize.h"

#include "KunrealAPI.h"
#include "SceneManager.h"


/// <summary>
/// 기존의 FileLoad를 참고해서 만들 예정
/// </summary>

EpicTool::Deserialize::Deserialize()
{

}

EpicTool::Deserialize::~Deserialize()
{

}

void EpicTool::Deserialize::Initialize(std::string& deserialize)
{
	std::ifstream inputData(deserialize);
	
	if (inputData.is_open())
	{
		nlohmann::json jsonData;
		inputData >> jsonData;
		inputData.close();
		// json에 따라 오브젝트를 설정함
		for (const auto& jsonItem : jsonData)
		{
			std::string objectName = jsonItem["POD"]["name"];
			KunrealEngine::GameObject* object = nullptr;
			//if (KunrealEngine::GetCurrentScene()->GetGameObject(objectName) == nullptr)
			//{
				object = KunrealEngine::GetCurrentScene()->CreateObject(objectName);
			//}

			if (object)
			{
				// 커스텀컴포넌트

				std::string testName = objectName;

				KunrealEngine::Transform* trans = object->GetComponent<KunrealEngine::Transform>();

				trans->SetPosition(jsonItem["POD"]["transform"]["position_x"], jsonItem["POD"]["transform"]["position_y"], jsonItem["POD"]["transform"]["position_z"]);
				trans->SetRotation(jsonItem["POD"]["transform"]["rotation_x"], jsonItem["POD"]["transform"]["rotation_y"], jsonItem["POD"]["transform"]["rotation_z"]);
				trans->SetScale(jsonItem["POD"]["transform"]["scale_x"], jsonItem["POD"]["transform"]["scale_y"], jsonItem["POD"]["transform"]["scale_z"]);

				auto custom = jsonItem["POD"].find("customComponent");

				bool isPlayer = false;
				bool isBoss = false;
	
				if (custom != jsonItem["POD"].end() && !jsonItem["POD"]["customComponent"].empty())
				{
					auto player = jsonItem["POD"]["customComponent"].find("Player");
					auto camera = jsonItem["POD"]["customComponent"].find("Camera");
					auto kamen = jsonItem["POD"]["customComponent"].find("Kamen");
	
					if (player != jsonItem["POD"]["customComponent"].end() && !jsonItem["POD"]["customComponent"]["Player"].empty())
					{
						object->AddComponent<KunrealEngine::Player>();
						isPlayer = true;
					}
	
					if (kamen != jsonItem["POD"]["customComponent"].end() && !jsonItem["POD"]["customComponent"]["Kamen"].empty())
					{
						object->AddComponent<KunrealEngine::Kamen>();
						isBoss = true;
					}
	
					//카메라
	
					if (camera != jsonItem["POD"]["customComponent"].end() && !jsonItem["POD"]["customComponent"]["Camera"].empty())
					{
						float cameraPos[3] = { 0.0f, 30.0f, -55.0f };
						if (object->GetComponent<KunrealEngine::Camera>() == nullptr)
						{
							object->AddComponent<KunrealEngine::Camera>();
						}
						object->GetComponent<KunrealEngine::Camera>()->SetCameraPosition(cameraPos[0], cameraPos[1], cameraPos[2]);
						float targetPos[3] = { 0.0f, -15.0f, 0.0f };
						object->GetComponent<KunrealEngine::Camera>()->SetTargetPosition(targetPos[0], targetPos[1], targetPos[2]);
						
						if (object->GetObjectName() == "testCamera") // 카메라가 여러개인 경우를 대비한 임시코드, 처음에 메인으로 쓸 오브젝트를 미리 정해두는것도 좋겠다
						{
							object->GetComponent<KunrealEngine::Camera>()->SetMainCamera();
						}
					}
	
	
				}
				if (isPlayer != true || isBoss != true)
				{
					// 메쉬 관련
					auto meshRenderer = jsonItem["POD"].find("meshRenderer");
					auto mesh = jsonItem["POD"]["meshRenderer"].find("Mesh");
					auto diffuse = jsonItem["POD"]["meshRenderer"].find("Diffuse");
					auto normal = jsonItem["POD"]["meshRenderer"].find("Normal");
					auto renderingState = jsonItem["POD"]["meshRenderer"].find("RenderingState");

					if (meshRenderer != jsonItem["POD"].end() && !jsonItem["POD"]["meshRenderer"].empty() && object->GetComponent<KunrealEngine::Kamen>() == NULL)
					{
						if (object->GetComponent<KunrealEngine::MeshRenderer>() == nullptr)
						{
							object->AddComponent<KunrealEngine::MeshRenderer>();
						}

						if (mesh != jsonItem["POD"]["meshRenderer"].end() && (!jsonItem["POD"]["meshRenderer"]["Mesh"].empty()))
						{
							object->GetComponent<KunrealEngine::MeshRenderer>()->SetMeshObject(jsonItem["POD"]["meshRenderer"]["Mesh"][0].get<std::string>().c_str());
						}
						if (diffuse != jsonItem["POD"]["meshRenderer"].end() && (!jsonItem["POD"]["meshRenderer"]["Diffuse"].empty()))
						{
							int count = 0;
							for (auto& diffuseLoad : jsonItem["POD"]["meshRenderer"]["Diffuse"])
							{
								object->GetComponent<KunrealEngine::MeshRenderer>()->SetDiffuseTexture(count, diffuseLoad.get<std::string>().c_str());
								count++;
							}

						}
						if (normal != jsonItem["POD"]["meshRenderer"].end() && (!jsonItem["POD"]["meshRenderer"]["Normal"].empty()))
						{
							int count = 0;
							for (auto& normalLoad : jsonItem["POD"]["meshRenderer"]["Normal"])
							{
								object->GetComponent<KunrealEngine::MeshRenderer>()->SetNormalTexture(count, normalLoad.get<std::string>().c_str());
								count++;
							}
						}
						if (renderingState != jsonItem["POD"]["meshRenderer"].end() && jsonItem["POD"]["meshRenderer"]["RenderingState"] == "\u0000")
						{
							object->GetComponent<KunrealEngine::MeshRenderer>()->SetActive(false);
						}

					}


					// 라이트

					auto light = jsonItem["POD"].find("light");
					auto lightType = jsonItem["POD"]["lightType"];

					if (light != jsonItem["POD"].end() && !jsonItem["POD"]["light"].empty())
					{
						if (object->GetComponent<KunrealEngine::Light>() == nullptr)
						{
							object->AddComponent<KunrealEngine::Light>();
						}
						std::string check = jsonItem["POD"]["lightType"]["LightType"].get<std::string>().c_str();
						if (check == "DirectionalLight") // DirectionalLight
						{
							//auto를 벡터로 명시적으로 선언할지 확인 
							auto ambirentX = jsonItem["POD"]["light"]["Ambient_x"];
							auto ambirentY = jsonItem["POD"]["light"]["Ambient_y"];
							auto ambirentZ = jsonItem["POD"]["light"]["Ambient_z"];
							auto ambirentW = jsonItem["POD"]["light"]["Ambient_w"];

							auto diffuseX = jsonItem["POD"]["light"]["Diffuse_x"];
							auto diffuseY = jsonItem["POD"]["light"]["Diffuse_y"];
							auto diffuseZ = jsonItem["POD"]["light"]["Diffuse_z"];
							auto diffuseW = jsonItem["POD"]["light"]["Diffuse_w"];

							auto specularX = jsonItem["POD"]["light"]["Specular_x"];
							auto specularY = jsonItem["POD"]["light"]["Specular_y"];
							auto specularZ = jsonItem["POD"]["light"]["Specular_z"];
							auto specularW = jsonItem["POD"]["light"]["Specular_w"];

							auto directionX = jsonItem["POD"]["light"]["Direction_x"];
							auto directionY = jsonItem["POD"]["light"]["Direction_y"];
							auto directionZ = jsonItem["POD"]["light"]["Direction_z"];

							object->GetComponent<KunrealEngine::Light>()->CreateDirectionalLight();

							object->GetComponent<KunrealEngine::Light>()->SetAmbient(ambirentX, ambirentY, ambirentZ, ambirentW);
							object->GetComponent<KunrealEngine::Light>()->SetDiffuse(diffuseX, diffuseY, diffuseZ, diffuseW);
							object->GetComponent<KunrealEngine::Light>()->SetSpecular(specularX, specularY, specularZ, specularW);
							object->GetComponent<KunrealEngine::Light>()->SetDirection(directionX, directionY, directionZ);
							object->GetComponent<KunrealEngine::Light>()->SetActive(true);  //고려중
						}
						else if (check == "PointLight")
						{
							auto ambirentX = jsonItem["POD"]["light"]["Ambient_x"];
							auto ambirentY = jsonItem["POD"]["light"]["Ambient_y"];
							auto ambirentZ = jsonItem["POD"]["light"]["Ambient_z"];
							auto ambirentW = jsonItem["POD"]["light"]["Ambient_w"];

							auto diffuseX = jsonItem["POD"]["light"]["Diffuse_x"];
							auto diffuseY = jsonItem["POD"]["light"]["Diffuse_y"];
							auto diffuseZ = jsonItem["POD"]["light"]["Diffuse_z"];
							auto diffuseW = jsonItem["POD"]["light"]["Diffuse_w"];

							auto range = jsonItem["POD"]["light"]["Range"];

							auto specularX = jsonItem["POD"]["light"]["Specular_x"];
							auto specularY = jsonItem["POD"]["light"]["Specular_y"];
							auto specularZ = jsonItem["POD"]["light"]["Specular_z"];
							auto specularW = jsonItem["POD"]["light"]["Specular_w"];

							object->GetComponent<KunrealEngine::Light>()->CreatePointLight();

							object->GetComponent<KunrealEngine::Light>()->SetAmbient(ambirentX, ambirentY, ambirentZ, ambirentW);
							object->GetComponent<KunrealEngine::Light>()->SetDiffuse(diffuseX, diffuseY, diffuseZ, diffuseW);
							object->GetComponent<KunrealEngine::Light>()->SetPointRange(range);
							object->GetComponent<KunrealEngine::Light>()->SetSpecular(specularX, specularY, specularZ, specularW);
						}

					}
					// 이미지

					auto image = jsonItem["POD"].find("image");


					if (image != jsonItem["POD"].end() && !jsonItem["POD"]["image"].empty())
					{
						auto imageName = jsonItem["POD"]["image"]["ImageName"];
						auto imageStatus = jsonItem["POD"]["image"]["ImageStatus"];

						if (object->GetComponent<KunrealEngine::ImageRenderer>() == nullptr)
						{
							object->AddComponent<KunrealEngine::ImageRenderer>();
						}

						object->GetComponent<KunrealEngine::ImageRenderer>()->SetImage(imageName);

						if (imageStatus == "True")
						{
							object->GetComponent<KunrealEngine::ImageRenderer>()->SetImageStatus(true);
						}
						else if (imageStatus == "False")
						{
							object->GetComponent<KunrealEngine::ImageRenderer>()->SetImageStatus(false);
						}
					}


					// 콜라이더

					auto collider = jsonItem["POD"].find("collider");

					if (collider != jsonItem["POD"].end() && !jsonItem["POD"]["collider"].empty())
					{
						if (object->GetComponent<KunrealEngine::BoxCollider>() == nullptr)
						{
							object->AddComponent<KunrealEngine::BoxCollider>();
						}

						object->GetComponent<KunrealEngine::BoxCollider>()->SetOffset(jsonItem["POD"]["collider"]["Offset_x"],
							jsonItem["POD"]["collider"]["Offset_y"],
							jsonItem["POD"]["collider"]["Offset_z"]);

						object->GetComponent<KunrealEngine::BoxCollider>()->SetBoxSize(jsonItem["POD"]["collider"]["BoxSize_x"],
							jsonItem["POD"]["collider"]["BoxSize_y"],
							jsonItem["POD"]["collider"]["BoxSize_z"]);

					}

					//사운드

					auto sound_Name = jsonItem["POD"].find("sound_Name");


					if (sound_Name != jsonItem["POD"].end() && !jsonItem["POD"]["sound_Name"].empty()) // 사운드 파일이 존재하는 Path는 반드시 필요하기에 해당 조건으로 충분
					{
						if (object->GetComponent<KunrealEngine::SoundPlayer>() == nullptr)
						{
							object->AddComponent<KunrealEngine::SoundPlayer>();
						}

						for (int i = 0; i < jsonItem["POD"]["sound_Name"].size(); ++i)
						{
							std::string sound_Name = jsonItem["POD"]["sound_Name"][i];
							bool sound_Is3D = jsonItem["POD"]["sound_Is3D"][i];
							bool sound_IsLoop = jsonItem["POD"]["sound_IsLoop"][i];
							int sound_Volume = jsonItem["POD"]["sound_Volume"][i];

							int  listIndex = object->GetComponent<KunrealEngine::SoundPlayer>()->CreateSoundInfo(sound_Name, sound_Is3D, sound_IsLoop, sound_Volume);
							object->GetComponent<KunrealEngine::SoundPlayer>()->CreateSound(listIndex);
						}

					}

				}
	
			}
		}
	
		for (const auto& jsonItem : jsonData)
		{
	
			std::string objectName = jsonItem["POD"]["name"];
	
			KunrealEngine::GameObject* currentObject = KunrealEngine::GetCurrentScene()->GetGameObject(objectName);
	
			// 부모 오브젝트 설정
			// 차례로 오브젝트를 만들기에 부모쪽은 모든 처리가 끝나고 추가하는게 나아보임
	
			if (currentObject)
			{
				auto parent = jsonItem["POD"].find("parent");
	
				if (parent != jsonItem["POD"].end() && !jsonItem["POD"]["parent"].empty())
				{
					auto parent = jsonItem["POD"]["parent"].find("ParentName");
	
					if (parent != jsonItem["POD"]["parent"].end() && !jsonItem["POD"]["parent"]["ParentName"].empty())
					{
						KunrealEngine::GameObject* parentObject = KunrealEngine::GetCurrentScene()->GetGameObject(jsonItem["POD"]["parent"]["ParentName"]);
	
						currentObject->SetParent(parentObject);
					}
				}
			}
		}
	
	
	}
}
