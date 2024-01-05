#include "Scene.h"
#include "GameObject.h"

KunrealEngine::Scene::Scene()
	:_sceneName("")
{

}

KunrealEngine::Scene::Scene(std::string sceneName)
	:_sceneName(sceneName)
{
	
}

KunrealEngine::Scene::~Scene()
{

}

void KunrealEngine::Scene::Initialize()
{
	if (_objectList.empty())
	{
		//scene이 생성되면 카메라 오브젝트를 만들어줌
		CreateObject("MainCamera");
		///카메라 필요
		//GetGameObject("MainCamera")->AddComponent<ObjectCamera>();
		//기본 카메라 오브젝트가 가진 카메라를 메인 카메라로 설정
		SetMainCamera(GetGameObject("MainCamera"));
	}
}

void KunrealEngine::Scene::Finalize()
{
	for (auto objects : _objectList)
	{
		objects->Finalize();
		delete objects;
	}

	_objectList.clear();
}

void KunrealEngine::Scene::FixedUpdate()
{
	if (!_objectList.empty())
	{
		for (auto& objects : _objectList)
		{
			if (objects->GetActivated())
			{
				objects->FixedUpdate();
			}
		}
	}
}

void KunrealEngine::Scene::Update()
{
	if (!_objectList.empty())
	{
		for (auto& objects : _objectList)
		{
			if (objects->GetActivated())
			{
				objects->Update();
			}
		}
	}
}

void KunrealEngine::Scene::LateUpdate()
{
	if (!_objectList.empty())
	{
		for (auto& objects : _objectList)
		{
			if (objects->GetActivated())
			{
				objects->LateUpdate();
			}
		}
	}
}

void KunrealEngine::Scene::Render()
{
	/// 그래픽스와 이야기해서 수정 예정
}

KunrealEngine::GameObject* KunrealEngine::Scene::CreateObject()
{
	GameObject* gameObject = new GameObject();
	_objectList.emplace_back(gameObject);
	gameObject->Initialize();
	SortObjects();

	return gameObject;
}

KunrealEngine::GameObject* KunrealEngine::Scene::CreateObject(std::string objectName)
{
	GameObject* gameObject = new GameObject(objectName);
	gameObject->SetObjectName(objectName);
	_objectList.emplace_back(gameObject);
	gameObject->Initialize();
	SortObjects();

	return gameObject;
}

void KunrealEngine::Scene::AddObject(GameObject* obj)
{
	GameObject* gameObject = obj;					// 복사대입연산자
	_objectList.emplace_back(gameObject);
	SortObjects();
}

void KunrealEngine::Scene::DeleteGameObject(std::string objectName)
{
	auto iter = find_if(_objectList.begin(), _objectList.end(), [&](GameObject* object)
		{
			return object->GetObjectName() == objectName;
		});

	if (iter != _objectList.end())
	{
		(*iter)->Finalize();						// 컴포넌트를 비워주고
		delete* iter;								// 삭제하고
		_objectList.erase(iter);					// 컨테이너에서 빼준다
	}
	else
	{
		/// 전달받은 이름의 오브젝트가 없음 툴에 표시
		return;
	}
}

void KunrealEngine::Scene::DeleteGameObject(GameObject* obj)
{
	auto iter = find(_objectList.begin(), _objectList.end(), obj);

	if (iter != _objectList.end())
	{
		obj->Finalize();
		delete* iter;
		_objectList.erase(iter);
	}
	else
	{
		/// 전달받은 이름의 오브젝트가 없음 툴에 표시 예정
		return;
	}
}

std::string KunrealEngine::Scene::GetSceneName()
{
	return this->_sceneName;
}

std::vector<KunrealEngine::GameObject*> KunrealEngine::Scene::GetObjectList()
{
	return this->_objectList;
}

KunrealEngine::GameObject* KunrealEngine::Scene::GetGameObject(std::string objectName)
{
	// 오브젝트 리스트에서 전달받은 이름을 가진 오브젝트가 있는지 확인
	auto iter = find_if(_objectList.begin(), _objectList.end(), [&](GameObject* object)
		{
			return object->GetObjectName() == objectName;
		});

	if (iter != _objectList.end())
	{
		return *iter;
	}
	else
	{
		/// 해당 오브젝트가 없다는 표시 예정
		return nullptr;
	}
}

void KunrealEngine::Scene::SetMainCamera(GameObject* obj)
{
	/// 그래픽스의 카메라를 받은 후 구현 예정
}

void KunrealEngine::Scene::SortObjects()
{
	if (_objectList.size() > 0)
	{
		for (int i = 0, j = 0; i < _objectList.size(); i++)
		{
			GameObject* obj = _objectList[i];

			for (j = i - 1; j >= 0 && _objectList[i]->GetLayer() > obj->GetLayer(); j--)
			{
				_objectList[j + 1] = _objectList[j];
			}

			_objectList[j + 1] = obj;
		}
	}
}
