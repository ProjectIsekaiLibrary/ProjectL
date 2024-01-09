/// <summary>
/// Camera 객체에서 
/// 게임 엔진쪽에서 사용할 함수를 모아놓은 인터페이스 클래스
/// 
/// 윤종화
/// </summary>

#pragma once

namespace KunrealEngine
{
	namespace KunrealMath
	{
		struct Float2;
		struct Float3;
	}
}

namespace GInterface
{
	class GraphicsCamera
	{
	public:
		virtual ~GraphicsCamera() {};

	public:
		// 카메라 객체 삭제
		virtual void Delete() abstract;

	public:
		// 카메라 위치 설정
		virtual void SetCameraPosition(KunrealEngine::KunrealMath::Float3 position) abstract;
		// 카메라가 바라보는 타겟 위치 설정
		virtual void SetTargetPosition(KunrealEngine::KunrealMath::Float3 position) abstract;
		// 카메라 회전 angle.x (위아래), angle.y (좌우)
		virtual void RotateCamera(KunrealEngine::KunrealMath::Float2 angle) abstract;

	public:
		// 카메라 좌우 이동
		virtual void Strafe(float deltaTime) abstract;
		// 카메라 앞뒤 이동
		virtual void Walk(float deltaTime) abstract;
		// 카메라 위아래 이동
		virtual void UpDown(float deltaTime) abstract;
	};
}