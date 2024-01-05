#include "GameClient.h"
#include "../KunrealEngine/KunrealAPI.h"

KunrealEngine::EngineCore* engineInstance = KunrealEngine::CreateEngine();

GameClient::GameClient::GameClient()
	:_hWnd(), _MSG(), _windowPosX(0), _windowPosY(0), _screenWidth(1920), _screenHeight(1080),
	_core(nullptr)
{

}

GameClient::GameClient::~GameClient()
{

}

HRESULT GameClient::GameClient::Initialize(HINSTANCE hInstance)
{
	/// Win32 관련
	// 윈도 클래스
	wchar_t szAppName[] = L"ProjectL";
	WNDCLASSEXW wcex{};

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = this->WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = szAppName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	// 윈도 클래스 등록
	RegisterClassExW(&wcex);

	// 윈도 생성
	_hWnd = CreateWindowW(szAppName, szAppName,
		WS_OVERLAPPEDWINDOW,
		_windowPosX, _windowPosY, _screenWidth, _screenHeight,
		nullptr, nullptr, hInstance, NULL);


	if (!_hWnd)
	{
		return S_FALSE;
	}
	// 생성된 윈도를 화면에 표시
	ShowWindow(_hWnd, SW_SHOWNORMAL );
	UpdateWindow(_hWnd);

	//여기에 게임엔진 객체 생성 및 초기화

	engineInstance->Initialize(_hWnd, hInstance, _screenWidth, _screenHeight);
	KunrealEngine::CreateScene("JEONG");
	KunrealEngine::CreateScene("KI");
	KunrealEngine::CreateScene("HOON");

	std::vector<KunrealEngine::Scene*>* _inner = KunrealEngine::GetSceneLists();

	//KunrealEngine::GetCurrentScene()->CreateObject("JINSOOMAN");

	std::string jinsoo = KunrealEngine::GetCurrentScene()->GetSceneName();

	KunrealEngine::ChangeScene("HOON");

	return S_OK;
}

void GameClient::GameClient::UpdateAll()
{
	//엔진의 업데이트 부분이 들어감
	engineInstance->UpdateAll();
}

LRESULT CALLBACK GameClient::GameClient::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC	hdc{};
	PAINTSTRUCT ps{};

	switch (message)
	{
		case WM_ACTIVATE:

			return 0;

		case WM_SIZE:
		{
			// 창 크기 변환
		}
		return 0;

		case WM_ENTERSIZEMOVE:
		{
			// 창 크기 관련;
		}
		return 0;

		case WM_EXITSIZEMOVE:
		{
			// 창 크기 관련;
		}
		return 0;

		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:

			return 0;

		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:

			return 0;

		case WM_MOUSEMOVE:


		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void GameClient::GameClient::GameLoop()
{
	while (true)
	{
		if (PeekMessage(&_MSG, NULL, 0, 0, PM_REMOVE))
		{
			if (_MSG.message == WM_QUIT) break;

			DispatchMessage(&_MSG);
		}
		else
		{
			UpdateAll();
		}
	}
}

void GameClient::GameClient::Finalize()
{
	//메모리 해제할 부분들 엔진의 Finalize도 여기에
	engineInstance->Finalize();
}
