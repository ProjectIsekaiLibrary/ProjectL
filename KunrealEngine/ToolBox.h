#pragma once
#include "CommonHeader.h"
#include <DirectXMath.h>

/// <summary>
/// 게임로직을 보조하는 클래스
/// 함수는 모두 static
/// </summary>

namespace KunrealEngine
{
	class ToolBox
	{
		/// 랜덤 관련
	public:
		// 0부터 매개변수의 수까지 랜덤
		static int GetRandomNum(int maxNum);

		// 범위 랜덤
		static int GetRandomNum(int minNum, int maxNum);

		// float 값 random 반환
		static float GetRandomFloat(float minNum, float maxNum);

		/// 수학 관련
	public:
		// 두 XMFLOAT3 좌표 사이의 거리
		static float GetDistance(DirectX::XMFLOAT3 src, DirectX::XMFLOAT3 dst);

		// 두 XMFLOAT3 좌표 사잇각
		static float GetAngle(DirectX::XMFLOAT3 src, DirectX::XMFLOAT3 dst);

		// 쿼터니언을 오일러로
		static DirectX::XMFLOAT3 QuaternionToEulerAngles(const DirectX::XMVECTOR& quaternion);
	};

}
