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
	bool ispick = _image->IsPicked(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y);

	if (_focusimage != nullptr)
	{
		if (ispick && !_focused)
		{
			_focused = true;
			_focusimage->GetOwner()->SetActive(true);
		}

		else if(!ispick && _focused)
		{
			_focused = false;
			_focusimage->GetOwner()->SetActive(false);
		}

		else if (!_focused && _focusimage->GetOwner()->GetActivated())
		{
			_focusimage->GetOwner()->SetActive(false);
		}
	}

	if (InputSystem::GetInstance()->MouseButtonDown(0))
	{
		if (ispick)
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

void KunrealEngine::ButtonSystem::Setfocused(ImageRenderer* focusimage)
{
	_focusimage = focusimage;
}
