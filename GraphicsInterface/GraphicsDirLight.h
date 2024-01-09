/// <summary>
/// DirectionalLight 인터페이스
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
	class GraphicsDirLight : public  GInterface::GraphicsLight
	{
	public:
		virtual ~GraphicsDirLight() {};

	public:
		// ambient 값 설정
		virtual void SetAmbient(KunrealEngine::KunrealMath::Float4 ambient) abstract;
		// diffuse 값 설정
		virtual void SetDiffuse(KunrealEngine::KunrealMath::Float4 diffuse) abstract;
		// specular 값 설정
		virtual void SetSpecular(KunrealEngine::KunrealMath::Float4 specular) abstract;
		// direction 값 설정
		virtual void SetDirection(KunrealEngine::KunrealMath::Float3 direction) abstract;
	};
}