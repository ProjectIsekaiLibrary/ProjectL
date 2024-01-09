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
/// </summary>
void EpicTool::FileLoad::Initialize()  // 아래 부분은 이후에 반복적으로 호출 될 수 있는 부분이기에 분리할 수도 있다 , 
{
    ////json파일 중에 앞서 저장해둔 파일을 읽어옴
    //std::ifstream inputFile("editor_stte.json");
    //if (inputFile.is_open()) // 해당 부분 안쪽으로 진입을 못함
    //{
    //    nlohmann::json jsonData;
    //    inputFile >> jsonData;
    //    inputFile.close();
    //    // json에 따라 오브젝트를 설정함
    //    for (const auto& jsonItem : jsonData)
    //    {
    //        std::string sceneName = jsonItem["SceneName"];

    //        std::string objectName = jsonItem["ObjectName"];  // 이름 갖고있기

    //        std::string mainCamera = "MainCamera";

    //        {
    //        	////존재 안하면 만들고 설정  // 해당부분 문제 발생 (주석처리하면 없는 오브젝트는 안만들고 중복으로 만듬)
				//KunrealEngine::CreateScene(sceneName);
				//if (objectName != mainCamera)
				//{
				//	KunrealEngine::GetScene(sceneName)->CreateObject(objectName);
				//}

    //        	KunrealEngine::GameObject* object = KunrealEngine::GetCurrentScene()->GetGameObject(objectName); 
    //        	std::vector<KunrealEngine::GameObject*> objectIndex = KunrealEngine::GetCurrentScene()->
    //        		GetObjectList();

    //            if(object)
    //        	{    
    //        		// 오브젝트의 데이터를 가져옴
    //        		// 오브젝트가 있다면 트랜스폼을 가져옴
    //        		KunrealEngine::Transform* trans = object->GetComponent<KunrealEngine::Transform>();
    //                
    //        		// json 값에 따라 Transform을 설정해줌
    //        		trans->SetPosition(jsonItem["TransformData"]["position_x"], jsonItem["TransformData"]["position_y"], jsonItem["TransformData"]["position_z"]);
    //        		trans->SetRotation(jsonItem["TransformData"]["rotation_x"], jsonItem["TransformData"]["rotation_y"], jsonItem["TransformData"]["rotation_z"]);
    //        		trans->SetScale(jsonItem["TransformData"]["scale_x"], jsonItem["TransformData"]["scale_y"], jsonItem["TransformData"]["scale_z"]);

    //                // json값에 따라 mesh component를 설정해줌

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
