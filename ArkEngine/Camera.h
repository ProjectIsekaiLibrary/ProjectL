/// <summary>
/// 아크엔진에서 사용할 카메라
/// DirectXMath를 기반으로 만듬
/// 
/// 윤종화
/// </summary>

#pragma once
#include <vector>
#include "ICamera.h"
#include "GraphicsCamera.h"

namespace DirectX
{
	struct XMFLOAT3;
	struct XMFLOAT4;
	struct XMFLOAT4X4;
	struct XMMATRIX;
}

namespace ArkEngine
{
	namespace ArkDX11
	{
		class Transform;

		class Camera : public ICamera, public GInterface::GraphicsCamera
		{
		public:
			Camera(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, DirectX::XMFLOAT3& worldUp);
			~Camera();

		public:
			virtual void Update() override;

		public:
			virtual bool GetMain() override;
			virtual void SetMain(bool tf) override;

		public:
			virtual void Delete() override;

		public:
			// 투영행렬 설정
			virtual void SetProjectionMatrix(float fieldOfView_Y, float aspect, float nearZ, float farZ, bool isPerspective) override;

			// 카메라 좌우로 이동
			virtual void Strafe(float deltaTime) override;

			// 카메라 앞뒤로 이동
			virtual void Walk(float deltaTime) override;

			// 카메라 위아래로 이동
			virtual void UpDown(float deltaTime) override;

		public:
			virtual std::vector<DirectX::XMFLOAT4>& GetFrstum() override;

		public:
			virtual DirectX::XMFLOAT3 GetCameraPosition() override;
			virtual void SetCameraPosition(DirectX::XMFLOAT3 position) override;
			virtual void SetTargetPosition(DirectX::XMFLOAT3 position) override;
			virtual void RotateCamera(DirectX::XMFLOAT2 angle) override;


		public:
			// View 행렬 가져오기
			virtual const DirectX::XMMATRIX GetViewMatrix() override;

			// 투영 행렬 가져오기
			virtual const DirectX::XMMATRIX GetProjMatrix() override;

			// 카메라의 위치 가져오기
			virtual const DirectX::XMFLOAT3 GetCameraPos() override;

		private:
			// pos, target, worldUp을 통해 look, right, up 백터를 설정 후 view 행렬 업데이트
			void LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, DirectX::XMFLOAT3& worldUp);
			
			// 설저된 look, right, up 백터를 사용하여 view 행렬 업데이트
			void UpdateViewMatrix();

			// 프러스텀 업데이트
			void UpdateFrustum();
			
		private:
			DirectX::XMFLOAT3 _positionVector;
			DirectX::XMFLOAT3 _targetVector;

			DirectX::XMFLOAT3 _lookVector;
			DirectX::XMFLOAT3 _rightVector;
			DirectX::XMFLOAT3 _upVector;

			DirectX::XMFLOAT4X4 _viewMatrix;
			DirectX::XMFLOAT4X4 _projectionMatrix;

		private:
			bool _isMain;
			std::vector<DirectX::XMFLOAT4> _frustumPlanes;
		};
	}
}

