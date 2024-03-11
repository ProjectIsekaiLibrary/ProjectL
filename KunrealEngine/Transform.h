#pragma once

#include <DirectXMath.h>
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

		friend class GameObject;
	private:
		Transform();
	public:
		~Transform();

	public:
		virtual void Initialize() override;
		virtual void Release() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		virtual void OnTriggerEnter() override;
		virtual void OnTriggerStay() override;
		virtual void OnTriggerExit() override;

		virtual void SetActive(bool active) override;

	private:
		DirectX::XMFLOAT3 _position;
		DirectX::XMFLOAT4 _rotation;				// local
		DirectX::XMFLOAT3 _scale;

		DirectX::XMFLOAT4 _quaternion;				// world
		DirectX::XMFLOAT4X4 _worldTM;

	public:
		// Rotation값 조정
		void SetRotation(float x, float y, float z);

		// Position값 조정
		void SetPosition(float x, float y, float z);

		// Scale값 조정
		void SetScale(float x, float y, float z);

		DirectX::XMFLOAT3 GetPosition();
		DirectX::XMFLOAT4 GetRotation();
		DirectX::XMFLOAT3 GetScale();

	public:
		// WorldTM 계산			// SRT
		DirectX::XMFLOAT4X4 CreateWorldTransformMatrix();
		DirectX::XMFLOAT4X4 GetWorldTM();
	};
}

#pragma warning (default: 4251)