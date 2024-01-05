#include "GameObject.h"
#include "Component.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Transform.h"

#include "../KunrealMath/MathHeaders.h"

#pragma comment(lib,"../Bin/x64/Debug/KunrealMath.lib")

KunrealEngine::GameObject::GameObject()
	:_isActivated(true), _objectName(""), _layer(0), _parent(nullptr), _transform(nullptr)
{
	SceneManager& smInstance = SceneManager::GetInstance();
	int objectCount = 0;
	_objectName = "GameObject";		// 아무 이름도 지정해주지 않았을 경우 GameObject라는 이름으로 생성

	for (auto object : smInstance.GetCurrentScene()->GetObjectList())
	{
		if (_objectName == object->GetObjectName())		// 중복 확인
		{
			objectCount++;			// 이름이 중복될 경우 뒤에 괄호와 숫자를 붙여 다른 이름으로 생성
			_objectName = _objectName + " (" + std::to_string(objectCount) + ")";
		}
	}
}

KunrealEngine::GameObject::GameObject(std::string name)
	:_isActivated(true), _objectName(name), _layer(0), _parent(nullptr), _transform(nullptr)
{
	SceneManager& smInstance = SceneManager::GetInstance();
	int objectCount = 0;

	// 중복 체크 알고리즘은 위와 동일
	for (auto object : smInstance.GetCurrentScene()->GetObjectList())
	{
		if (_objectName == object->GetObjectName())
		{
			objectCount++;
			_objectName = _objectName + " (" + std::to_string(objectCount) + ")";
		}
	}
}

KunrealEngine::GameObject::~GameObject()
{

}

void KunrealEngine::GameObject::Initialize()
{
	// 오브젝트가 생성되자마자 트랜스폼을 만들어서 넣어줌
	AddComponent<Transform>();
	_transform = GetComponent<Transform>();
}

void KunrealEngine::GameObject::Awake()
{

}

void KunrealEngine::GameObject::OnEnable()
{

}

void KunrealEngine::GameObject::Start()
{

}

void KunrealEngine::GameObject::FixedUpdate()
{
	/// if안에 반복문안에 if문.. 비효율적이다
	/// 오브젝트들의 상태를 체크 후 각각을 컨테이너로 나눠서 반복문을 돌리는 방법 구상중
	if (_isActivated)
	{
		for (auto& components : _componentContainer)
		{
			if (components->GetActivated())
			{
				components->FixedUpdate();
			}
		}
	}
}

void KunrealEngine::GameObject::Update()
{
	if (_isActivated)
	{
		for (auto& components : _componentContainer)
		{
			if (components->GetActivated()) components->Update();
		}
	}
}

void KunrealEngine::GameObject::LateUpdate()
{
	if (_isActivated)
	{
		for (auto& components : _componentContainer)
		{
			if (components->GetActivated()) components->LateUpdate();
		}
	}
}

void KunrealEngine::GameObject::OnDisable()
{

}

void KunrealEngine::GameObject::Finalize()
{
	ClearComponent();
}

std::string KunrealEngine::GameObject::GetObjectName()
{
	return this->_objectName;
}

void KunrealEngine::GameObject::SetObjectName(const std::string& name)
{
	this->_objectName = name;
}

void KunrealEngine::GameObject::SetParent(GameObject* obj)
{
	this->_parent = obj;
}

KunrealEngine::GameObject* KunrealEngine::GameObject::GetParent()
{
	if (_parent != nullptr)
	{
		return _parent;
	}
	else
	{
		return nullptr;
	}
}

void KunrealEngine::GameObject::SetActive(bool active)
{
	this->_isActivated = active;

	for (auto& components : _componentContainer)
	{
		components->SetActive(active);
	}
}

bool KunrealEngine::GameObject::GetActivated()
{
	return this->_isActivated;
}

void KunrealEngine::GameObject::SetLayer(int layer)
{
	this->_layer = layer;
}

int KunrealEngine::GameObject::GetLayer() const
{
	return this->_layer;
}

void KunrealEngine::GameObject::SetTag(const std::string& tag)
{
	this->_tag = tag;
}

std::string KunrealEngine::GameObject::GetTag()
{
	return this->_tag;
}

std::string KunrealEngine::GameObject::GetOriginalTypeName(std::string name)
{
	std::string typeName = name;

	const std::string classPrefix = "class ";
	const std::string structPrefix = "struct ";

	if (typeName.substr(0, classPrefix.size()) == classPrefix)			// 클래스인가
	{
		typeName = typeName.substr(classPrefix.size());
	}
	else if (typeName.substr(0, structPrefix.size()) == structPrefix)	// 구조체인가
	{
		typeName = typeName.substr(structPrefix.size());
	}

	size_t colon = typeName.rfind("::");								// 네임스페이스 찾아서 제거
	if (colon != std::string::npos)
	{
		typeName = typeName.substr(colon + 2);
	}

	return typeName;
}

void KunrealEngine::GameObject::DeleteComponent(Component* component)
{
	auto iter = find(_componentContainer.begin(), _componentContainer.end(), component);

	if (iter != _componentContainer.end())			// 찾는게 있으면
	{
		(*iter)->Finalize();						// 컴포넌트에서 해제할게 있으면 해주고
		delete* iter;								// 해당 컴포넌트를 삭제
		_componentContainer.erase(iter);			// 컨테이너에서 비워줌
	}
}

void KunrealEngine::GameObject::ClearComponent()
{
	for (Component* component : _componentContainer)
	{
		component->Finalize();						// 해제할 내용이 있으면 먼저 해제해주고
		delete component;							// clear하기전에 delete 다 해줘야함
	}

	_componentContainer.clear();
}

std::vector<KunrealEngine::Component*> KunrealEngine::GameObject::GetComponentList()
{
	return this->_componentContainer;
}
