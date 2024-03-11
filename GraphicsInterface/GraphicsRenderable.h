/// <summary>
/// Renderable 객체에서 
/// 게임 엔진쪽에서 사용할 함수를 모아놓은 인터페이스 클래스
/// 
/// 윤종화
/// </summary>

#pragma once
#include "GraphicsStruct.h"

namespace DirectX
{
	struct XMFLOAT3;
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
		// 피킹 가능 여부 설정
		virtual void SetPickable(bool tf) abstract;
		// 피킹 가능 여부 가져오기
		virtual bool GetPickable() abstract;

	public:
		// 렌더러블 객체의 트랜스폼 설정								// WorldTM
		virtual void SetTransform(DirectX::XMFLOAT4X4 matrix) abstract;
		// 렌더러블 객체의 위치값 설정
		virtual void SetPosition(float x =0.0f, float y=0.0f, float z=0.0f) abstract;
		// 렌더러블 객체의 회전값 설정
		virtual void SetRotation(float x = 0.0f, float y = 0.0f, float z = 0.0f) abstract;
		// 렌더러블 객체의 크기값 설정
		virtual void SetScale(float x = 1.0f, float y = 1.0f, float z = 1.0f) abstract;

		// 렌더러블 객체의 그릴 오브젝트 설정
		virtual void SetModel(const char* fileName) abstract;
		// 렌더러블 객체의 디퓨즈 텍스쳐 설정
		virtual void SetDiffuseTexture(int index, const char* textureName) abstract;
		// 렌더러블 객체의 노말 텍스쳐 설정
		virtual void SetNormalTexture(int index, const char* textureName) abstract;
		// 렌더러블 객체의 emissive 텍스쳐 설정
		virtual void SetEmissiveTexture(int index, const char* textureName) abstract;
		// 렌더러블 객체의 메테리얼 가져오기
		virtual const GInterface::Material GetMaterial() abstract;
		// 렌더러블 객체의 메테리얼 설정
		virtual void SetMaterial(GInterface::Material material) abstract;
		// 렌더러블 객체의 반사값 설정
		virtual void SetReflect(DirectX::XMFLOAT4 reflect) abstract;

	public:
		virtual const std::vector<std::string> GetDiffuseTextureList() abstract;
		virtual const std::vector<std::string> GetNormalTextureList() abstract;

	public:
		// 렌더러블 객체에 애니메이션 정보가 있다면 실행
		virtual void PlayAnimation(float deltaTime, bool continiousPlay = false) abstract;
		// 실행중인 애니메이션 멈추기
		virtual void StopAnimation() abstract;
		// FBX용 애니메이션 실행 함수 (인덱스로), 실행중인지 여부를 반환
		virtual bool PlayAnimation(float speed, float deltaTime, int animIndex, bool continuousPlay = false) abstract;
		// FBX용 애니메이션 실행 함수 (이름으로), 실행중인지 여부를 반환
		virtual bool PlayAnimation(float speed, float deltaTime, std::string animName, bool continuousPlay = false) abstract;
		// Mesh가 가지고 있는 애니메이션 클립 이름들 반환
		virtual const std::vector<std::string>& GetClipNames() abstract;
		// 애니메이션을 현재 프레임에서 멈춘다
		virtual void PauseAnimation() abstract;
		// 멈춘 애니메이션을 다시 재생한다
		virtual void ReplayAnimation() abstract;
		// 현재 프레임을 가져온다
		virtual float GetCurrentFrame() abstract;
		// 현재 애니메이션의 최대 프레임을 가져온다
		virtual float GetMaxFrame() abstract;
	};
}