#pragma once
#include <dinput.h>
#include "CommonHeader.h"

#pragma comment(lib, "dinput8")
#pragma comment(lib, "dxguid")

// https://www.notion.so/KEY-CODE-LIST-3fe3e429fa224b77b8699b6568e052f4 참고

enum class KEY;

namespace KunrealEngine
{
	class _DECLSPEC InputSystem
	{
	private:
		InputSystem();
		~InputSystem();

	private:
		IDirectInput8* _input;
		IDirectInputDevice8* _keyboard;
		IDirectInputDevice8* _mouse;
		LPVOID _cKey;

		char _keybuffer[256] = {};		// 현 프레임에 대한 키버퍼
		char _prevkeybuffer[256] = {};	// 이전 프레임에 대한 키버퍼

		HWND _hWnd;

		DIMOUSESTATE _mouseState;
		BYTE _prevrgbButtons[4] = {};
		int _mouseWheelData;			// DIMOUSESTATE의 lz값이 휠 입력값이다. 
		// 이 변수에 lz 값을 더하는 식으로 휠이 얼마나 움직였는지 알 수 있다.
		int _prevmouseWheelData;
		//int _screenHeight;
		//int _screenWidth;
		int _mouseX;
		int _mouseY;

	public:
		// 인스턴스에 접근할 수 있는 메서드
		static InputSystem* GetInstance();

		// 입력시스템 초기화 함수
		bool Initialize(HINSTANCE instance, HWND hwnd, int screenHeight, int screenWidth);
		void Finalize();

		// 매 프레임마다 작동. 지금은 디바이스와의 연결이 끊어졌는지 아닌지를 체크. 
		void Update(float fTime);

		// 입력된 버튼을 bool타입으로 반환 (눌렸는가 안눌렸는가)
		bool WINAPI KeyInput(KEY keycode);
		bool WINAPI KeyUp(KEY keycode);
		bool WINAPI KeyDown(KEY keycode);

		// 입력된 마우스를 bool타입으로 반환 (눌렸는가 안눌렸는가)
		bool WINAPI MouseButtonInput(int button);
		bool WINAPI MouseButtonUp(int button);
		bool WINAPI MouseButtonDown(int button);

		// 마우스 휠값을 bool 타입으로 반환
		bool WINAPI WheelUp();
		bool WINAPI WheelDown();

		// 매개변수 xpos, ypos에 마우스의 좌표를 전달하는 함수.
		void GetMousePosition(int& xpos, int& ypos);

		// 매개변수 curState, prevState에 휠의 현재값, 이전프레임값을 전달하는 함수.
		void GetWheelState(int& curState, int& prevState);
		int GetWheelState();
	};
}