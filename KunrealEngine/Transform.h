#pragma once

#include "CommonHeader.h"
#include "Component.h"
#include "../KunrealMath/MathHeaders.h"

#pragma comment(lib,"../Bin/x64/Debug/KunrealMath.lib")
#pragma warning (disable: 4251)

namespace KunrealEngine
{
	class GameObject;

	class _DECLSPEC Transform : public Component
	{
	typedef bool FLAG;

	public:
		Transform();
		~Transform();

	public:
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
		/// DX Math를 사용하지 않기 위해
		KunrealMath::Float3 _position;
		KunrealMath::Float3 _rotation;
		KunrealMath::Float3 _scale;

		KunrealMath::Float2 _UIPosition;
		KunrealMath::Float2 _UIScale;

		KunrealMath::Matrix4x4 _worldTM;

	public:
		// Rotation값 조정
		void SetRotation(float x, float y, float z);

		// Position값 조정
		void SetPosition(float x, float y, float z);

		// Scale값 조정
		void SetScale(float x, float y, float z);

		KunrealMath::Float3 GetPosition();
		KunrealMath::Float3 GetRotation();
		KunrealMath::Float3 GetScale();

	public:
		/// UI전용 Transform 값
		// UI 전용 2D 좌표값
		void SetUIPosition(float x, float y);
		KunrealMath::Float2 GetUIPosition();

		// UI 전용 2D 스케일
		void SetUIScale(float x, float y);
		KunrealMath::Float2 GetUIScale();

	public:
		// WorldTM 계산			// SRT
		void CreateWorldTransformMatrix();
		KunrealMath::Matrix4x4 GetWorldTM();
	};
}

#pragma warning (default: 4251)