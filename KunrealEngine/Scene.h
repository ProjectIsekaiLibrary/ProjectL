#pragma once
#include "CommonHeader.h"

/// <summary>
/// Scene
/// 
/// 
/// </summary>

#pragma warning(disable: 4251)

namespace KunrealEngine
{
	class GameObject;
	// 카메라도 필요

	class _DECLSPEC Scene
	{	// 씬매니저에서만 씬을 생성할 수 있게
		friend class SceneManager;
	private:
		Scene();
		Scene(std::string sceneName);	// scene의 이름을 string으로 매개변수로 받아서 바로 설정
		~Scene();

	public:
		void Initialize();

		// scene이 담고 있는 오브젝트들을 삭제해주고
		// scene 자체에 대한 메모리 해제는 SceneManager에서 해준다
		void Finalize();
		void FixedUpdate();
		void Update();
		void LateUpdate();

		void Render();

	private:
		std::vector<GameObject*> _objectList;	// 현재 scene에 담긴 오브젝트들
		std::string _sceneName;					// scene을 구분지을 이름

	public:
		// scene에 오브젝트를 만들어주는 함수
		GameObject* CreateObject();

		// scene에 오브젝트를 만들어주는 함수
		// 매개변수로 이름을 미리 받아 object이름으로 넣어줌
		GameObject* CreateObject(std::string objectName);

		// 이미 만들어진 오브젝트를 scene에 추가
		void AddObject(GameObject* obj);

		// scene에 있는 오브젝트를 삭제
		void DeleteGameObject(std::string objectName);
		void DeleteGameObject(GameObject* obj);

		// scene의 이름을 string으로 반환
		std::string GetSceneName();

		// scene이 가지고 있는 오브젝트들의 목록을 반환
		std::vector<GameObject*> GetObjectList();

		// 전달받은 이름의 특정 오브젝트를 반환
		GameObject* GetGameObject(std::string objectName);

		// 카메라 오브젝트 중 전달받은 오브젝트를 메인 카메라로 설정
		void SetMainCamera(GameObject* obj);

	private:
		// 오브젝트가 가진 Layer값을 기준으로 scene에 존재하는 오브젝트들을 정렬
		// Layer값이 낮은 오브젝트가 먼저 렌더됌 -> 밑으로 깔린다
		void SortObjects();
	};
}

#pragma warning (default: 4251)