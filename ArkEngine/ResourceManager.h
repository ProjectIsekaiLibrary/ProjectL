/// <summary>
/// 리소스 관리를 위한 매니저
/// 렌더링에 필요한 리소스들을 지니고 있을 예정
/// 
/// 윤종화
/// </summary>

#pragma once
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include "DXTKFont.h"
#include "IResource.h"
#include "IRenderable.h"
#include "IDebugObject.h"
#include "ICamera.h"
#include "ICubeMap.h"

class ASEParser;

struct TextPosition {
	int x;
	int y;

	// 비교 연산자 구현
	bool operator==(const TextPosition& other) const {
		return x == other.x && y == other.y;
	}
};

// Point 구조체를 사용하는 해시 함수 구현
namespace std
{
	template <>
	struct hash<TextPosition>
	{
		std::size_t operator()(const TextPosition& p) const
		{
			std::size_t h1 = std::hash<int>()(p.x);
			std::size_t h2 = std::hash<int>()(p.y);
			// 해시 함수를 조합하여 최종 해시 값을 반환
			return h1 ^ (h2 << 1);
		}
	};
}


namespace ArkEngine
{
	class ResourceManager
	{
	private:
		ResourceManager();
		~ResourceManager();

		static ResourceManager* instance;

	public:
		static ResourceManager* GetInstance();
		static void Delete();

	public:
		// IResource 형으로 저장되어있는 리소스를 원형으로 반환해줌
		template <typename Resource>
		Resource* GetResource(const std::string& fileName);

		// IResource 형으로 리소스를 백터에 저장함
		void SetResource(const std::string& fileName, IResource* resource);

		// 폴더에서 특정 확장자의 파일을 모두 찾아 그 이름들을 백터로 넘겨줌
		std::vector<std::string> FindSpecificResources(const std::string& folderName, const std::string& extension);

		// 백터에 있는 특정 리소스를 삭제
		void DeleteResource(const std::string& fileName);

	public:
		// 렌더링 할 오브젝트들은 담아놓은 백터 반환
		const std::vector<ArkEngine::IRenderable*> GetRenderableList();

		// 렌더링 할 오브젝트들을 담아놓은 백터
		void AddRenderable(ArkEngine::IRenderable* renderable);

		// 특정 렌더링하던 오브젝트를 삭제
		void DeleteRenderable(ArkEngine::IRenderable* renderable);

	public:
		// 모든 디버그 오브젝트들은 담아놓은 백터 반환
		const std::vector<ArkEngine::IDebugObject*> GetDebugObjectList();

		void AddDebugObject(ArkEngine::IDebugObject* object);

		void DeleteDebugObject(ArkEngine::IDebugObject* object);

	public:
		// 모든 큐브맵을 담아놓은 백터 반환
		const std::unordered_set<ArkEngine::ICubeMap*> GetCubeMapList();

		std::vector<std::string> GetCubeMapNameList();

		// 현재 렌더링되고 있는 큐브맵을 반환
		ArkEngine::ICubeMap* GetNowCubeMap();

		void SetNowCubeMap(ArkEngine::ICubeMap* cubeMap);

		void SetCubeMapNameList(std::vector <std::string> nameVector);

		// 큐브맵 백터에 큐브맵을 추가
		void AddCubeMap(ArkEngine::ICubeMap* cubeMap);

		// 큐브맵 백터에서 큐브맵을 삭제
		void DeleteCubeMap(ArkEngine::ICubeMap* cubeMap);

	public:
		// 렌더링 할 오브젝트들은 담아놓은 백터 반환
		const std::vector<ArkEngine::ICamera*> GetCameraList();

		// 렌더링 할 오브젝트들을 담아놓은 백터
		void AddCamera(ArkEngine::ICamera* camera);

		// 특정 렌더링하던 오브젝트를 삭제
		void DeleteCamera(ArkEngine::ICamera* camera);

	public:
		const std::unordered_map<TextPosition, std::string>& GetTextList();
		// 텍스트 리스트에 텍스트 추가
		void SetTextList(int posX, int posY, std::string text);

	public:
		ASEParser* GetASEParser(const std::string& fileName);

		void AddASEParser(const std::string& fileName, ASEParser* parser);

		void DeleteASEParser(const std::string& fileName);

	public:
		// 에디터에 모델 이름을 넘겨주기 위함
		std::vector<std::string> GetRenderableNameList();

		void SetTextureNameList(std::string name);

		// 에디터에 텍스쳐 이름을 넘겨주기 위함
		const std::vector<std::string> GetTextureNameList();

	public:
		void ReleaseAll();

	private:
		std::unordered_map<std::string, IResource*> _resourceList;

	private:
		// 존재하는 모든 그릴 수 있는 오브젝트들
		std::vector<IRenderable*> _renderableList;

		std::vector<IDebugObject*> _debugList;

		// 존재하는 모든 큐브맵
		ICubeMap* _nowCubeMap;

		std::unordered_set<ICubeMap*> _cubeMapList;

		std::vector<std::string> _cubeMapNameList;

		// 존재하는 모든 카메라 오브젝트들
		std::vector<ICamera*> _cameraList;

		// 출력할 텍스트 리스트
		std::unordered_map<TextPosition, std::string> _textList;

		// ASE 파서 리스트
		std::unordered_map<std::string, ASEParser*> _ASEParserList;

	private:
		// 존재하는 모든 텍스쳐 이름 리스트 // 갖고있는게 비효율적인가 고민 중
		std::vector<std::string> _textureNameList;
	};

	template <typename Resource>
	Resource* ArkEngine::ResourceManager::GetResource(const std::string& fileName)
	{
		auto iter = _resourceList.find(fileName);

		if (iter != _resourceList.end())
		{
			return dynamic_cast<Resource*>(_resourceList.at(fileName));
		}
		else
		{
			return nullptr;
		}
	}
}

