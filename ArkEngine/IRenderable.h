/// <summary>
/// 
/// 오브젝트를 생성할 때 필수 요소를 담은 인터페이스
/// 
/// 윤종화
/// </summary>

#pragma once

namespace ArkEngine
{
	class ICamera;
}

namespace ArkEngine
{
	class IRenderable abstract
	{
	public:
		IRenderable() {};
		virtual ~IRenderable() {};

	public:
		virtual void Initialize() abstract;

		virtual void Update(ArkEngine::ICamera* camera) abstract;

		virtual void Render() abstract;

		virtual void Finalize() abstract;

	public:
		virtual bool GetRenderingState() abstract;
		virtual void SetRenderingState(bool tf) abstract;

	public:
		virtual void SetPosition(float x = 0.0f, float y = 0.0f, float z = 0.0f) abstract;
		virtual void SetRotation(float x = 0.0f, float y = 0.0f, float z = 0.0f) abstract;
		virtual void SetScale(float x = 1.0f, float y = 1.0f, float z = 1.0f) abstract;
		virtual void SetDiffuseTexture(int index, const char* textureName) abstract;
		virtual void SetNormalTexture(int index, const char* textureName) abstract;
		virtual void SetEmissiveTexture(int index, const char* textureName) abstract;

		virtual void SetAnimator() abstract;

	public:
		virtual void PlayAnimation(float deltaTime, bool continiousPlay) abstract;
		virtual void StopAnimation() abstract;

	public:
		virtual bool GetPickable() abstract;
		virtual void SetPickable(bool tf) abstract;

		virtual unsigned int GetHashID() abstract;

	public:
		virtual bool GetInsideFrustumState() abstract;
	};
}