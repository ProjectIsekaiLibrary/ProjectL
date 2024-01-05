#pragma once
#include "Component.h"
#include "../KunrealMath/MathHeaders.h"

namespace GInterface
{
	class GraphicsCamera;
}

#pragma warning(disable: 4251)

namespace KunrealEngine
{
	class Transform;

	class _DECLSPEC Camera : public Component
	{
	public:
		Camera();
		~Camera();

		virtual void Initialize() override;
		virtual void Finalize() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		virtual void OnTriggerEnter() override;
		virtual void OnTriggerStay() override;
		virtual void OnTriggerExit() override;

		virtual void SetActive(bool active) override;

	private:
		// 카메라 객체
		GInterface::GraphicsCamera* _camera;

		// 부모 오브젝트의 트랜스폼 객체
		Transform* _transform;

		// 타겟 포지션
		KunrealMath::Float3 _targetPosition;

		// 그래픽스 엔진 카메라의 strafe, walk등의 움직임을 위한 변수
		KunrealMath::Float3 _prevPosition;

		// 그래픽스 엔진 카메라의 Rotate를 위한 변수
		KunrealMath::Float3 _prevRotation;

		// 카메라 움직이는 방식
		// 타겟을 계속 바라볼 것이냐 포지션만 움직일 것이냐
		bool _fixTarget;

	public:
		// 컴포넌트가 소유하고 있는 카메라 반환
		GInterface::GraphicsCamera* GetCamera();
		
		// 카메라의 위치 설정
		void SetCameraPosition(float posX, float posY, float posZ);

		// 카메라의 타겟 위치 설정
		void SetTargetPosition(float posX, float posY, float posZ);

		// 카메라의 각도 전환
		void RotateCamera(float pitch, float yaw);

		// 카메라 좌우 이동
		void CameraStrafe(float deltaTime);

		// 카메라 앞뒤 이동
		void CameraWalk(float deltaTime);

		// 카메라 상하 이동
		void CameraUpDown(float deltaTime);

	public:
		/// 조작 방식에 따른 카메라의 움직임
		// 타겟을 고정하지 않음
		void MoveCamera();

		// 타겟을 고정함
		void MoveFixedCamera();

		// 타겟을 고정하지 않도록 설정
		void UnfixCamera();

		// 타겟을 고정하도록 설정
		// 이걸 실행하면 이 컴포넌트에 저장된 타겟의 포지션으로 바라보도록 설정함
		// 다른 위치를 바라보게 하고 싶으면 SetTargetPosition으로 타겟 포지션을 재설정 해줘야함
		void FixCamera();

	public:
		// 이 컴포넌트가 들고있는 카메라를 메인 카메라로 설정함
		void SetMainCamera();
	};
}

#pragma warning(default : 4251)
