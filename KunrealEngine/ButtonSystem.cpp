#include "ButtonSystem.h"
#include "InputSystem.h"

KunrealEngine::ButtonSystem::ButtonSystem()
	: _image(nullptr)
{
}

KunrealEngine::ButtonSystem::~ButtonSystem()
{
}

void KunrealEngine::ButtonSystem::Initialize()
{
}

void KunrealEngine::ButtonSystem::Release()
{
}

void KunrealEngine::ButtonSystem::FixedUpdate()
{
}

void KunrealEngine::ButtonSystem::Update()
{
	if (InputSystem::GetInstance()->MouseButtonDown(0))
	{
		if (_image->IsPicked(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y))
		{
			_script();
		}
	}
}

void KunrealEngine::ButtonSystem::LateUpdate()
{
}

void KunrealEngine::ButtonSystem::OnTriggerEnter()
{
}

void KunrealEngine::ButtonSystem::OnTriggerStay()
{
}

void KunrealEngine::ButtonSystem::OnTriggerExit()
{
}

void KunrealEngine::ButtonSystem::SetActive(bool active)
{
}

void KunrealEngine::ButtonSystem::SetButtonFunc(std::function<void()> func)
{
	_script = func;
}

void KunrealEngine::ButtonSystem::SetImage(ImageRenderer* image)
{
	_image = image;
}
