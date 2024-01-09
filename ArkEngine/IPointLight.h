/// <summary>
/// 
/// 그래픽스 인터페이스와 PointLight와의
/// 연동을 위한 클래스
/// 
/// 윤종화
/// </summary>

#pragma once
#include "GraphicsPointLight.h"

namespace ArkEngine
{
	class IPointLight : public GInterface::GraphicsPointLight
	{
	public:
		IPointLight(unsigned int lightIndex);
		~IPointLight();

	public:
		virtual void Delete() override;

	public:
		// ambient 값 설정
		virtual void SetAmbient(KunrealEngine::KunrealMath::Float4 ambient) override;
		// diffuse 값 설정
		virtual void SetDiffuse(KunrealEngine::KunrealMath::Float4 diffuse) override;
		// specular 값 설정
		virtual void SetSpecular(KunrealEngine::KunrealMath::Float4 specular) override;
		// position 값 설정
		virtual void SetPosition(KunrealEngine::KunrealMath::Float3 position) override;
		// ragne 값 설정
		virtual void SetRange(float range) override;

	public:
		virtual bool GetActive() override;

		virtual void SetActive(bool ox) override;

	public:
		int GetActiveIndex();
		void MinusIndex();
		void MinusActiveIndex();

	private:
		unsigned int _index;
		int _activeIndex;
		bool _isActive;
	};
}

