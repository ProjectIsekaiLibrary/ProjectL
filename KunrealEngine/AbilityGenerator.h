#pragma once
#include "CommonHeader.h"
#include "Ability.h"

/// <summary>
/// 플레이어가 사용할 스킬 정보를 생성해주는 클래스
/// 어디서든 불리는게 아니라 PlayerAbility 쪽에서만 불리게 하고 싶은데..
/// </summary>

namespace KunrealEngine
{
	class PlayerAbility;	

	class AbilityGenerator
	{
	private:
		AbilityGenerator();
		~AbilityGenerator();

		PlayerAbility* _paComponent;

		// 싱글톤
	public:
		static AbilityGenerator& GetInstance()
		{
			static AbilityGenerator _instance;
			return _instance;
		}

		// playerability 쪽에서 플레이어 객체에 대한 정보를 넘겨줌
		// pa가 가진 컨테이너에 만들어진 스킬 정보를 넣어줌
		void SetPlayerAbility(PlayerAbility* pa);

		Ability* Shot();
		void ShotLogic();
	};
}