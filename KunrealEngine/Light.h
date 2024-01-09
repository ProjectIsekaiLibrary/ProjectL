#pragma once
#include "Component.h"
#include "GraphicsSystem.h"
#include "../KunrealMath/MathHeaders.h"

/// <summary>
/// Directional light, Point light, spotlight 종류가 다양하다
/// 
/// 개별 컴포넌트로 만들 것인가 아니면 하나의 컴포넌트로 전환이 가능하게 할 것인가..
/// </summary>

#pragma warning(disable: 4251)

namespace KunrealEngine
{
	class Transform;

	enum LightType
	{
		DirectionalLight,
		PointLight,
		SpotLight,
		None
	};

	class _DECLSPEC Light : public Component
	{

	public:
		Light();
		~Light();

		virtual void Initialize() override;
		virtual void Finalize() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		virtual void OnTriggerEnter() override;
		virtual void OnTriggerStay() override;
		virtual void OnTriggerExit() override;

		// light는 기본 state가 false여서 켜줘야함
		virtual void SetActive(bool active) override;

	private:
		// 어떤 light인지 관리용
		LightType _type;

		// 오브젝트의 Transform
		Transform* _transform;

	public:
		LightType GetLightType();

	private:
		// light 종류들의 공통 부모
		GInterface::GraphicsLight* _light;

	public:
		// light가 생성되었는지 확인
		bool GetLightStatus();

	private:
		// Directional Light에 필요한 정보들
		KunrealMath::Float4 _ambient;
		KunrealMath::Float4 _diffuse;
		KunrealMath::Float4 _specular;
		KunrealMath::Float3 _direction;

	private:
		// PointLight에 필요한 정보들
		//KunrealMath::Float3 _positionP;	// 포지션은 따로 관리할 필요가 없음
		float _rangeP;

	public:
	/// Light 공통 함수
	/// Ambient, Diffuse, Specular는 모든 종류의 빛에 들어간다
	/// Light 종류별로 캐스팅해서 사용
	
		void SetAmbient(float x, float y, float z, float w);
		KunrealMath::Float4 GetAmbient();

		void SetDiffuse(float x, float y, float z, float w);
		KunrealMath::Float4 GetDiffuse();

		void SetSpecular(float x, float y, float z, float w);
		KunrealMath::Float4 GetSpecular();

	public:
		/// Directional Light 관련
		// Directional light 생성
		// 디폴트 매개변수로 모든 값은 0
		void CreateDirectionalLight(KunrealMath::Float4 ambient = { 0.f, 0.f, 0.f, 0.f }, KunrealMath::Float4 diffuse = { 0.f, 0.f, 0.f, 0.f }, KunrealMath::Float4 specular = { 0.f, 0.f, 0.f, 0.f }, KunrealMath::Float3 direction = { 0.f, 0.f, 0.f });

		void SetDirection(float x, float y, float z);
		KunrealMath::Float3 GetDirection();

	public:
		/// PointLight 관련
		// PointLight 생성
		// 디폴트 매개변수로 모든 값은 0
		// 포지션은 빛의 위치이므로 transform에서 가져오면 되니까 제외
		void CreatePointLight(KunrealEngine::KunrealMath::Float4 ambient = {0.f, 0.f, 0.f, 0.f}, KunrealEngine::KunrealMath::Float4 diffuse = { 0.f, 0.f, 0.f, 0.f }, KunrealEngine::KunrealMath::Float4 specular = { 0.f, 0.f, 0.f, 0.f }, float range = 0.f);
	
		void SetPointPosition();

		void SetPointRange(float range);
		float GetPointRange();
	};
}

#pragma warning(default: 4251)