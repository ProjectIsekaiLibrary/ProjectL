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
		void Setfocused(ImageRenderer* focusimage);

	private: 
		ImageRenderer* _image;
		ImageRenderer* _focusimage;

		// 이미지가 포커스드인지 아닌지 구별하기 위한 변수
		// 매프레임 셋이미지를 하면 속도가 장난 아닐것 같아서 추가
		bool _focused;

		std::function<void()> _script;
	};
}