/// <summary>
/// PointLight 인터페이스
/// 
/// 이건호 윤종화
/// </summary>

#pragma once
#include "GraphicsLight.h"

namespace KunrealEngine
{
	namespace KunrealMath
	{
		struct Float3;
		struct Float4;
	}
}

namespace GInterface
{
	class GraphicsPointLight : public  GInterface::GraphicsLight
	{
	public:
		virtual ~GraphicsPointLight() {};

	public:
		// ambient 값 설정
		virtual void SetAmbient(KunrealEngine::KunrealMath::Float4 ambient) abstract;
		// diffuse 값 설정
		virtual void SetDiffuse(KunrealEngine::KunrealMath::Float4 diffuse) abstract;
		// specular 값 설정
		virtual void SetSpecular(KunrealEngine::KunrealMath::Float4 specular) abstract;
		// position 값 설정
		virtual void SetPosition(KunrealEngine::KunrealMath::Float3 position) abstract;
		// ragne 값 설정
		virtual void SetRange(float range) abstract;
	};
}