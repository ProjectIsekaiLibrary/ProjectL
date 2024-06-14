#pragma once
#include "Component.h"
#include "ImageRenderer.h"
#include "functional"

namespace KunrealEngine
{
	class ButtonSystem : public Component
	{
	public:
		ButtonSystem();
		~ButtonSystem();

		// Component의 가상함수들 오버라이드
		void Initialize() override;
		void Release() override;

		void FixedUpdate() override;
		void Update() override;
		void LateUpdate() override;

		void OnTriggerEnter() override;
		void OnTriggerStay() override;
		void OnTriggerExit() override;

		void SetActive(bool active) override;

		// 버튼 동작시 실행할 함수를 지정
		void SetButtonFunc(std::function<void()> func);
		void SetImage(ImageRenderer* image);

	private: 
		ImageRenderer* _image;
		std::function<void()> _script;
	};
}