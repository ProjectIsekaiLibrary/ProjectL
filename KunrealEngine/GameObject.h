#pragma once
#include "CommonHeader.h"

/// <summary>
/// 오브젝트 클래스
/// 오브젝트들은 이 클래스를 통해 생성되며, 오브젝트 종류별로 클래스를 만드는 것이 아니라
/// 모든 오브젝트는 이 클래스로 통일되고 이름, 컴포넌트로 구분된다
/// 
/// 이건호
/// </summary>

#pragma warning(disable: 4251)

namespace KunrealEngine
{
	class Component;
	class Transform;

	class _DECLSPEC GameObject
	{
		friend class Scene;
		friend class SceneManager;
	private:							// 직접 생성할 수 없고, scene을 통해서 생성
		GameObject();
		GameObject(std::string name);
		~GameObject();

	private:
		// 모든 과정은 scene, 혹은 LifeCycle에 의해 수행
		void Initialize();				// 네이밍은 고민중
		void Awake();
		void OnEnable();
		void Start();

		void FixedUpdate();
		void Update();
		void LateUpdate();

		void OnDisable();
		void Finalize();

	private: 
		std::vector<Component*> _componentContainer;		// 컴포넌트들을 담을 벡터
		bool _isActivated;									// 활성화 여부
		std::string _objectName;							// 오브젝트 이름
		std::string _sceneName;								// 어떤 scene에 들어있는가
		Transform* _transform;								// 오브젝트가 transform이 없을 수 없으니 미리 넣어줌
		GameObject* _parent;								// 부모 오브젝트
		int _layer;											// 렌더 순서와 관련된 레이어
		std::string _tag;									// 오브젝트를 비교할 태그

	public:
		// 오브젝트의 이름을 반환
		std::string GetObjectName();

		// 오브젝트의 이름 설정
		void SetObjectName(const std::string& name);

		// 부모 오브젝트 설정
		void SetParent(GameObject* obj);

		// 부모 오브젝트 반환
		GameObject* GetParent();

		// 활성화 여부를 정해주는 함수
		// true = 활성화 / false = 비활성화
		void SetActive(bool active);

		// 오브젝트의 활성화 여부를 반환 true / false
		bool GetActivated();

		// 렌더 순서와 관련된 레이어를 설정하는 함수
		// 낮을 수록 먼저 렌더 -> 밑으로 깔림
		void SetLayer(int layer);

		// 레이어 수치를 반환하는 함수
		int GetLayer() const;

		// 태그 설정
		void SetTag(const std::string& tag);

		// 태그 반환
		std::string GetTag();

	/// 컴포넌트 관련
	private:
		// 템플릿에 typeid.Name()을 사용하면 class 네임스페이스::클래스 이름 <- 이렇게 반환함
		// class와 네임스페이스 이름을 제거하고 원본 이름을 반환해주는 함수
		static std::string GetOriginalTypeName(std::string name);

	public:
		// 오브젝트에 컴포넌트를 추가하는 함수
		template<typename T>
		void AddComponent()
		{
			T* newComponent = new T();

			this->_componentContainer.emplace_back(newComponent);
			if (newComponent->GetOwner() == nullptr)
			{
				newComponent->SetOwner(this);											// 컴포넌트의 소유주를 이 오브젝트로
				newComponent->_componentName = GetOriginalTypeName(typeid(T).name());	// 컴포넌트의 이름을 넣어줌
			}
			newComponent->Initialize();
		}

		// 오브젝트의 특정 컴포넌트를 반환하는 함수
		// componentContainer는 오브젝트 내에서의 컨테이너
		template<typename T>
		T* GetComponent()
		{
			for (int i = 0; i < _componentContainer.size(); i++)
			{
				// dynamic_cast는 형변환 실패시 nullptr를 반환한다
				Component* component = dynamic_cast<T*>(_componentContainer[i]);
				if (component != nullptr)			// 찾는게 있어서 nullptr가 아닐 경우
				{
					return (T*)component;			// 컴포넌트를 전달받은 T 형식으로 변환해서 리턴
				}
			}

			return nullptr;
		}

		// 특정 컴포넌트를 삭제하는 함수
		void DeleteComponent(Component* component);

		// 오브젝트에 할당된 모든 컴포넌트를 제거하는 함수
		void ClearComponent();

		// 컴포넌트 리스트를 반환
		std::vector<Component*> GetComponentList();
	};
}

#pragma warning(default: 4251)