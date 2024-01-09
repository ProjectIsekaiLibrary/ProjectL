/// <summary>
/// 
/// 카메라를 생성할 때 필수 요소를 담은 인터페이스
/// 
/// 2023.09.26 YJH
/// </summary>

#pragma once
namespace ArkEngine
{
	class ICamera abstract
	{
	public:
		ICamera() {};
		virtual ~ICamera() {};

	public:
		virtual bool GetMain() abstract;
		virtual void SetMain(bool tf) abstract;

	public:
		virtual void Delete() abstract;

	public:
		virtual void SetProjectionMatrix(float fovY, float aspect, float nz, float fz, bool isPerspective) abstract;

		virtual void Strafe(float deltaTime) abstract;
		virtual void Walk(float deltaTime) abstract;
		virtual void UpDown(float deltaTime) abstract;
	};
}