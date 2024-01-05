#include "GraphicsSystem.h"
#include "../GraphicsInterface/GraphicsHeaders.h"

KunrealEngine::GraphicsSystem::GraphicsSystem()
	:_graphics(nullptr)
{

}

KunrealEngine::GraphicsSystem::~GraphicsSystem()
{

}

KunrealEngine::GraphicsSystem& KunrealEngine::GraphicsSystem::GetInstance()
{
	static GraphicsSystem _instance;
	return _instance;
}

void KunrealEngine::GraphicsSystem::Initialize(HWND hwnd, int screenWidth, int screenHeight)
{
	_graphics = CreateGraphicsEngine();
	_graphics->Initialize(reinterpret_cast<long long>(hwnd), screenWidth, screenHeight);
}

void KunrealEngine::GraphicsSystem::Finalize()
{
	ReleaseGraphicsEngine(_graphics);
}

GInterface::GraphicsInterface* KunrealEngine::GraphicsSystem::GetGraphics()
{
	return this->_graphics;
}

void KunrealEngine::GraphicsSystem::CreateCubeMap(const char* cubeMapName, const char* textureName /*= ""*/, bool isCube /*= false*/)
{
	return this->_graphics->CreateCubeMap(cubeMapName, textureName, isCube);
}

void KunrealEngine::GraphicsSystem::DeleteCubeMap(const char* cubeMapName)
{
	return this->_graphics->DeleteCubeMap(cubeMapName);
}

void KunrealEngine::GraphicsSystem::SetMainCubeMap(std::string cubeMapName)
{
	return this->_graphics->SetMainCubeMap(cubeMapName);
}

const std::vector<std::string> KunrealEngine::GraphicsSystem::GetCubeMapList()
{
	if (!_graphics->GetCubeMapList().empty())
	{
		return this->_graphics->GetCubeMapList();
	}
}

const std::vector<std::string> KunrealEngine::GraphicsSystem::GetRenderableList()
{
	return this->_graphics->GetRenderableNameList();
}

const std::vector<std::string> KunrealEngine::GraphicsSystem::GetTextureList()
{
	return this->_graphics->GetTextureNameList();
}