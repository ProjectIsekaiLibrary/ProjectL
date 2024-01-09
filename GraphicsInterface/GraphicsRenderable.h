/// <summary>
/// Renderable 객체에서 
/// 게임 엔진쪽에서 사용할 함수를 모아놓은 인터페이스 클래스
/// 
/// 윤종화
/// </summary>

#pragma once
#include "GraphicsStruct.h"

namespace KunrealEngine
{
	namespace KunrealMath
	{
		struct Float4;
	}
}

namespace GInterface
{
	class GraphicsRenderable
	{
	public:
		virtual ~GraphicsRenderable() {};

	public:
		// 렌더러블 객체 삭제
		virtual void Delete() abstract;
		// 현재 렌더링 여부 가져오기
		virtual bool GetRenderingState() abstract;
		// 렌더링 여부 설정
		virtual void SetRenderingState(bool tf) abstract;

	public:
		// 렌더러블 객체의 트랜스폼 설정								// WorldTM
		virtual void SetTransform(KunrealEngine::KunrealMath::Matrix4x4 matrix) abstract;
		// 렌더러블 객체의 위치값 설정
		virtual void SetPosition(float x =0.0f, float y=0.0f, float z=0.0f) abstract;
		// 렌더러블 객체의 회전값 설정
		virtual void SetRotation(float x = 0.0f, float y = 0.0f, float z = 0.0f) abstract;
		// 렌더러블 객체의 크기값 설정
		virtual void SetScale(float x = 1.0f, float y = 1.0f, float z = 1.0f) abstract;

		// 렌더러블 객체의 그릴 오브젝트 설정
		virtual void SetModel(const char* fileName) abstract;
		// 렌더러블 객체의 디퓨즈 텍스쳐 설정
		virtual void SetDiffuseTexture(const char* textureName) abstract;
		// 렌더러블 객체의 노말 텍스쳐 설정
		virtual void SetNormalTexture(const char* textureName) abstract;	
		// 렌더러블 객체의 메테리얼 가져오기
		virtual const GInterface::Material GetMaterial() abstract;
		// 렌더러블 객체의 메테리얼 설정
		virtual void SetMaterial(GInterface::Material material) abstract;
		// 렌더러블 객체의 반사값 설정
		virtual void SetReflect(KunrealEngine::KunrealMath::Float4 reflect) abstract;


	public:
		// 렌더러블 객체에 애니메이션 정보가 있다면 실행
		virtual void PlayAnimation(float deltaTime, bool continiousPlay = false) abstract;
		// 실행중인 애니메이션 멈추기
		virtual void StopAnimation() abstract;
		// FBX용 애니메이션 실행 함수
		virtual void PlayFBXAnimation(float speed, int animIndex, bool continuousPlay = false) abstract;
	};
}