#include <filesystem>
#include "ASEParser.h"
#include "ResourceManager.h"

ArkEngine::ResourceManager* ArkEngine::ResourceManager::instance = nullptr;

ArkEngine::ResourceManager::ResourceManager()
	: _nowCubeMap(nullptr)
{

}

ArkEngine::ResourceManager::~ResourceManager()
{

}


ArkEngine::ResourceManager* ArkEngine::ResourceManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new ArkEngine::ResourceManager();
	}

	return instance;
}


void ArkEngine::ResourceManager::Delete()
{
	delete instance;
}

void ArkEngine::ResourceManager::SetResource(const std::string& fileName, IResource* resource)
{
	_resourceList.insert(std::make_pair(fileName, resource));
}

std::vector<std::string> ArkEngine::ResourceManager::FindSpecificResources(const std::string& folderName, const std::string& extension)
{
	std::string folderPath = folderName + "/";
	std::string targetExtension = "." + extension;

	std::vector<std::string> filenameList;

	for (const auto& entry : std::filesystem::directory_iterator(folderPath))
	{
		if (entry.is_regular_file() && entry.path().extension() == targetExtension)
		{
			filenameList.emplace_back(folderPath + entry.path().filename().string());
		}
		else if (entry.is_directory())
		{
			std::vector<std::string> subfolderFiles = FindSpecificResources(entry.path().string(), extension);

			filenameList.insert(filenameList.end(), subfolderFiles.begin(), subfolderFiles.end());
		}
	}

	return filenameList;
}

void ArkEngine::ResourceManager::DeleteResource(const std::string& fileName)
{
	auto iter = _resourceList.find(fileName);

	if (iter != _resourceList.end())
	{
		delete iter->second;
		_resourceList.erase(iter);
	}
}

const std::vector<ArkEngine::IRenderable*> ArkEngine::ResourceManager::GetRenderableList()
{
	return _renderableList;
}

void ArkEngine::ResourceManager::AddRenderable(ArkEngine::IRenderable* renderable)
{
	_renderableList.emplace_back(renderable);
}

void ArkEngine::ResourceManager::DeleteRenderable(ArkEngine::IRenderable* renderable)
{
	_renderableList.erase(std::remove(_renderableList.begin(), _renderableList.end(), renderable), _renderableList.end());

	delete renderable;
}

const std::vector<ArkEngine::IDebugObject*> ArkEngine::ResourceManager::GetDebugObjectList()
{
	return _debugList;
}

void ArkEngine::ResourceManager::AddDebugObject(ArkEngine::IDebugObject* object)
{
	_debugList.emplace_back(object);
}

void ArkEngine::ResourceManager::DeleteDebugObject(ArkEngine::IDebugObject* object)
{
	_debugList.erase(std::remove(_debugList.begin(), _debugList.end(), object), _debugList.end());
}

const std::unordered_set<ArkEngine::ICubeMap*> ArkEngine::ResourceManager::GetCubeMapList()
{
	return _cubeMapList;
}

std::vector<std::string> ArkEngine::ResourceManager::GetCubeMapNameList()
{
	return _cubeMapNameList;
}

ArkEngine::ICubeMap* ArkEngine::ResourceManager::GetNowCubeMap()
{
	return _nowCubeMap;
}

void ArkEngine::ResourceManager::SetNowCubeMap(ArkEngine::ICubeMap* cubeMap)
{
	_nowCubeMap = cubeMap;
}

void ArkEngine::ResourceManager::SetCubeMapNameList(std::vector <std::string> nameVector)
{
	_cubeMapNameList = nameVector;
}

void ArkEngine::ResourceManager::AddCubeMap(ArkEngine::ICubeMap* cubeMap)
{
	for (auto index : _cubeMapList)
	{
		if (index->GetName() == cubeMap->GetName())
		{
			DeleteCubeMap(index);
			break;
		}
	}

	_cubeMapList.insert(cubeMap);
	_cubeMapNameList.emplace_back(cubeMap->GetName());
}

void ArkEngine::ResourceManager::DeleteCubeMap(ArkEngine::ICubeMap* cubeMap)
{
	_cubeMapNameList.erase(std::remove(_cubeMapNameList.begin(), _cubeMapNameList.end(), cubeMap->GetName()), _cubeMapNameList.end());
	_cubeMapList.erase(cubeMap);
	
	delete cubeMap;
}

const std::vector<ArkEngine::ICamera*> ArkEngine::ResourceManager::GetCameraList()
{
	return _cameraList;
}

void ArkEngine::ResourceManager::AddCamera(ArkEngine::ICamera* camera)
{
	_cameraList.emplace_back(camera);
}

void ArkEngine::ResourceManager::DeleteCamera(ArkEngine::ICamera* camera)
{
	if (camera->GetMain() == false)
	{
		_cameraList.erase(std::remove(_cameraList.begin(), _cameraList.end(), camera), _cameraList.end());
		
		delete camera;
	}

}

const std::unordered_map<TextPosition, std::string>& ArkEngine::ResourceManager::GetTextList()
{
	return _textList;
}

void ArkEngine::ResourceManager::SetTextList(int posX, int posY, std::string text)
{
	TextPosition temp;
	temp.x = posX;
	temp.y = posY;

	auto it = _textList.find(temp);

	if (it != _textList.end())
	{
		_textList.at(temp) = text;
	}
	else
	{
		_textList.insert(std::make_pair(temp, text));
	}
}

ASEParser* ArkEngine::ResourceManager::GetASEParser(const std::string& fileName)
{
	auto iter = _ASEParserList.find(fileName);

	if (iter != _ASEParserList.end())
	{
		return iter->second;
	}
	
	else return nullptr;
}

void ArkEngine::ResourceManager::AddASEParser(const std::string& fileName, ASEParser* parser)
{
	_ASEParserList.insert(std::make_pair(fileName, parser));
}

void ArkEngine::ResourceManager::DeleteASEParser(const std::string& fileName)
{
	auto iter = _ASEParserList.find(fileName);

	if (iter != _ASEParserList.end())
	{
		delete iter->second;
		_ASEParserList.erase(iter);
	}
}

std::vector<std::string> ArkEngine::ResourceManager::GetRenderableNameList()
{
	std::vector<std::string> nameList;

	for (const auto& pair : _ASEParserList)
	{
		nameList.emplace_back(pair.first);
	}

	return nameList;
}

const std::vector<std::string> ArkEngine::ResourceManager::GetTextureNameList()
{
	return _textureNameList;
}

void ArkEngine::ResourceManager::SetTextureNameList(std::string name)
{
	_textureNameList.emplace_back(name);
}

void ArkEngine::ResourceManager::ReleaseAll()
{
	_textList.clear();

	for (auto index : _renderableList)
	{
		delete index;
	}
	_renderableList.clear();

	_cubeMapNameList.clear();

	for (auto index : _cubeMapList)
	{
		delete index;
	}
	_cubeMapList.clear();

	for (auto index : _cameraList)
	{
		delete index;
	}
	_cameraList.clear();

	auto aseIt = _ASEParserList.begin();

	while (aseIt != _ASEParserList.end())
	{
		delete aseIt->second;

		aseIt->second = nullptr;
		aseIt = _ASEParserList.erase(aseIt);
	}

	auto it = _resourceList.begin();

	while (it != _resourceList.end())
	{
		it->second->UnLoad(it->first.c_str());
		it = _resourceList.begin();
	}
}