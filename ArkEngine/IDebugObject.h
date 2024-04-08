/// <summary>
/// 
/// 디버깅 오브젝트를 생성할 때 필수 요소를 담은 인터페이스
/// 
/// 윤종화
/// </summary>

#pragma once

namespace DirectX
{
	struct XMFLOAT4X4;
}

namespace ArkEngine
{
	class ICamera;
}

namespace ArkEngine
{
	class IDebugObject abstract
	{
	public:
		IDebugObject() {};
		virtual ~IDebugObject() {};

	public:
		virtual void Initialize() abstract;

		virtual void Update(ArkEngine::ICamera* camera) abstract;

		virtual void Render() abstract;

		virtual void Finalize() abstract;

	public:
		virtual bool GetRenderingState() abstract;
		virtual void SetRenderingState(bool tf) abstract;

	public:
		virtual void SetTransformMatrix(DirectX::XMFLOAT4X4 matrix) abstract;
		virtual void SetPosition(float x = 0.0f, float y = 0.0f, float z = 0.0f) abstract;
		virtual void SetRotation(float x = 0.0f, float y = 0.0f, float z = 0.0f) abstract;
		virtual void SetScale(float x = 1.0f, float y = 1.0f, float z = 1.0f) abstract;

	public:
		virtual void SetWorld(DirectX::XMFLOAT4X4 matrix) abstract;

	public:
		virtual float GetRadius() abstract;
	};
}