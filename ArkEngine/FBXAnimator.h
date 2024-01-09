/// FBX Animator 클래스
/// 게임엔진에 Animator 컴퍼넌트 만들기 위한 것
/// 
/// 김현재

#define MAX_MODEL_TRANSFORMS 96
#define MAX_MODEL_KEYFRAMES 500

#pragma once
#include <string>
#include <array>
#include "ParsingStructs.h"

struct AnimTransform
{
	// 250개의 관절정보를 담을 수 있다
	using TransformArrayType = std::array<Matrix, MAX_MODEL_TRANSFORMS>;

	// 2차 배열
	std::array<TransformArrayType, MAX_MODEL_KEYFRAMES> _transforms;
};

struct ModelAnimation;
struct ModelKeyframe;

namespace ArkEngine
{
	namespace ArkDX11
	{
		class FBXAnimator
		{
		public:
			FBXAnimator();
			~FBXAnimator();

		public:
			void PlayAnimationOnce(float speed, int animIndex);
			void PlayAnimationContinuous(float speed, int animIndex);

			void StopFBXAnimation();

		private:
			void ReadAnimData(std::wstring fileName);
			void UpdateAnimTransforms(float speed, unsigned int animIndex);

		private:
			std::vector<std::shared_ptr<ModelAnimation>> _animationIndex;
			std::shared_ptr<ModelAnimation> _anim;

			std::vector<AnimTransform> _animTransforms;
			std::shared_ptr<ModelKeyframe> _frame;

			unsigned int _currentAnimationIndex;
			float _animationSpeed;
			float _currentTime;
			bool _continuousPlay;

		};
	}
}