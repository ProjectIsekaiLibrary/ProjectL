/// <summary>
/// 그래픽스 엔진에서 사용하는
/// 
/// 
/// 윤종화
/// </summary>

#pragma once
#include "../KunrealMath/MathHeaders.h"

namespace GInterface
{
	struct Material
	{
		KunrealEngine::KunrealMath::Float4 ambient;
		KunrealEngine::KunrealMath::Float4 diffuse;
		KunrealEngine::KunrealMath::Float4 specular;
		KunrealEngine::KunrealMath::Float4 reflect;
	};
}
