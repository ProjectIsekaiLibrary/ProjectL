#include "FileLoad.h"

#include "KunrealAPI.h"
#include "SceneManager.h"

EpicTool::FileLoad::FileLoad()
{

}

EpicTool::FileLoad::~FileLoad()
{

}
/// <summary>
/// 리플랙션으로 대부분 폐기 예정
/// 2024.01.08
/// 
/// 컴포넌트 기반이라 디시리얼라이즈가 필요하다고 판단함
/// 2024.03.04
/// </summary>
void EpicTool::FileLoad::Initialize()  // 아래 부분은 이후에 반복적으로 호출 될 수 있는 부분이기에 분리할 수도 있다 , 
{
    //json파일 중에 앞서 저장해둔 파일을 읽어옴
   std::ifstream inputFile("output.json");
   if (inputFile.is_open())
   {
	   nlohmann::json jsonData;
	   inputFile >> jsonData;
	   inputFile.close();
	   // json에 따라 오브젝트를 설정함
	   for (const auto& jsonItem : jsonData)
	   {
		   std::string objectName = jsonItem["POD"]["name"];
		   KunrealEngine::GetCurrentScene()->CreateObject(objectName);

		   KunrealEngine::GameObject* object = KunrealEngine::GetCurrentScene()->GetGameObject(objectName);
		   std::vector<KunrealEngine::GameObject*> objectIndex = KunrealEngine::GetCurrentScene()->
			   GetObjectList();

		   if (object)
		   {
			   KunrealEngine::Transform* trans = object->GetComponent<KunrealEngine::Transform>();

			   trans->SetPosition(jsonItem["POD"]["position_x"], jsonItem["POD"]["position_y"], jsonItem["POD"]["position_z"]);
			   trans->SetRotation(jsonItem["POD"]["rotation_x"], jsonItem["POD"]["rotation_y"], jsonItem["POD"]["rotation_z"]);
			   trans->SetScale(jsonItem["POD"]["scale_x"], jsonItem["POD"]["scale_y"], jsonItem["POD"]["scale_z"]);


			   // 메쉬 관련
			   auto meshRenderer = jsonItem["POD"].find("meshRenderer");
			   auto diffuse = jsonItem["POD"]["meshRenderer"].find("Diffuse");
			   auto mesh = jsonItem["POD"]["meshRenderer"].find("Mesh");
			   auto normal = jsonItem["POD"]["meshRenderer"].find("Normal");
			   auto renderingState = jsonItem["POD"]["meshRenderer"].find("RenderingState");

			   if (meshRenderer != jsonItem["POD"].end() && !jsonItem["POD"]["meshRenderer"].empty())
			   {
				   object->AddComponent<KunrealEngine::MeshRenderer>();
				   KunrealEngine::MeshRenderer* meshRenderer = object->GetComponent<KunrealEngine::MeshRenderer>();

				   if (diffuse != jsonItem["POD"]["meshRenderer"].end() && (!jsonItem["POD"]["meshRenderer"]["Diffuse"].empty()))
				   {
					   object->GetComponent<KunrealEngine::MeshRenderer>()->SetDiffuseTexture(0, jsonItem["POD"]["meshRenderer"]["Diffuse"].get<std::string>().c_str());
				   }
				   if (mesh != jsonItem["POD"]["meshRenderer"].end() && (!jsonItem["POD"]["meshRenderer"]["Mesh"].empty()))
				   {
					   object->GetComponent<KunrealEngine::MeshRenderer>()->SetMeshObject(jsonItem["POD"]["meshRenderer"]["Mesh"].get<std::string>().c_str());
				   }
				   if (normal != jsonItem["POD"]["meshRenderer"].end() && (!jsonItem["POD"]["meshRenderer"]["Normal"].empty()))
				   {
					   // 노멀은 디퓨즈와 같은 이유로 보류
				   }
				   //if (renderingState != jsonItem["POD"]["meshRenderer"].end() && jsonItem["POD"]["meshRenderer"]["RenderingState"] == "\u0001")
				   //{
					  // object->GetComponent<KunrealEngine::MeshRenderer>()->
					  // 기본적으로 true라고 생각
				   //}

			   }


			   // 라이트

			   // 이미지

			   // 커스텀컴포넌트

			   // 애니메이션 프레임 (애니메이션 쪽이 ui 미구현이 많음)

		   }
	   }

   }

				//	auto meshRenderer = jsonItem["MeshData"].find("MeshRenderer");
				//	auto textureRenderer = jsonItem["MeshData"].find("TextureRenderer");

				//	if (meshRenderer != jsonItem["MeshData"].end() && !jsonItem["MeshData"]["MeshRenderer"].empty())
    //                {
				//		object->AddComponent<KunrealEngine::MeshRenderer>();
				//		KunrealEngine::MeshRenderer* mesh = object->GetComponent<KunrealEngine::MeshRenderer>();

				//		if (!jsonItem["MeshData"]["MeshRenderer"][0]["Mesh"].empty())
    //                    {
				//			if (!mesh->GetMeshStatus())
    //                        {
				//				std::string meshName = jsonItem["MeshData"]["MeshRenderer"][0]["Mesh"];
				//				mesh->SetMeshObject(meshName.c_str());

				//				if (jsonItem["MeshData"]["MeshRenderer"].size() > 1)
    //                            {
				//					mesh->SetActive(jsonItem["MeshData"]["MeshRenderer"][1]["RenderingState"]);
				//				}
				//			}
				//			else
    //                        {
				//				std::string meshName = jsonItem["MeshData"]["MeshRenderer"][0]["Mesh"];
				//				mesh->ChangeMeshObject(meshName.c_str());

				//				if (jsonItem["MeshData"]["MeshRenderer"].size() > 1)
    //                            {
				//					mesh->SetActive(jsonItem["MeshData"]["MeshRenderer"][1]["RenderingState"]);
				//				}
				//			}
				//		}
				//	}

				//	if (textureRenderer != jsonItem["MeshData"].end() && !jsonItem["MeshData"]["TextureRenderer"].empty())
    //                {
				//		object->AddComponent<KunrealEngine::MeshRenderer>();
				//		KunrealEngine::MeshRenderer* mesh = object->GetComponent<KunrealEngine::MeshRenderer>();

    //                    if (mesh->GetMeshStatus())
    //                    {
				//			if (!jsonItem["MeshData"]["TextureRenderer"][0]["Diffuse"].empty())
				//			{
				//				std::string diffuseTexture = jsonItem["MeshData"]["TextureRenderer"][0]["Diffuse"];
				//				mesh->SetDiffuseTexture(diffuseTexture.c_str());
				//			}

				//			if (jsonItem["MeshData"]["TextureRenderer"].size() > 1 && !jsonItem["MeshData"]["TextureRenderer"][1]["Normal"].empty())
				//			{
				//				std::string normalTexture = jsonItem["MeshData"]["TextureRenderer"][1]["Normal"];
				//				mesh->SetNormalTexture(normalTexture.c_str());
				//			}
				//		}
				//	}


    //        		objectIndex.emplace_back(object);
    //        	}
    //        }
    //    }
    //}
}

void EpicTool::FileLoad::Update()
{

}
