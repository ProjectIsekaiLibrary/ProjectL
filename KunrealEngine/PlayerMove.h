#pragma once
#include "KunrealAPI.h"
#include "Player.h"

namespace DirectX
{
	struct XMFLOAT3;
}

namespace KunrealEngine
{
	class _DECLSPEC PlayerMove : public Component
	{
	public:
		PlayerMove();
		~PlayerMove();

		void Initialize() override;
		void Release() override;

		void FixedUpdate() override;
		void Update() override;
		void LateUpdate() override;

		void OnTriggerEnter() override;
		void OnTriggerStay() override;
		void OnTriggerExit() override;

		void SetActive(bool active) override;

	private:
		Transform* _transform;					// 부모 오브젝트의 Transform
		DirectX::XMFLOAT3 _targetPos;			// 이동할 위치
		Player* _playerComp;					// 플레이어 컴포넌트
		float _errorRange;						// 이 정도 오차범위 이내로 움직이면 움직임이 완료된 것으로 판단
		int _nodeCount;							// 이동할 노드의 수
		float _movedRange;						// 대시로 이동한 거리 체크용 변수
		float _movableRange;					// 대시 가능 거리		// 노드로 계산
		float _posY;							// 맵마다 다른 높이를 적용할 변수

		// 이동할 위치를 마우스 우클릭 시 업데이트
		void UpdateTargetPosition();

		// 이동 명령시 노드 업데이트
		void UpdateMoveNode();

		// 대시 명령시 노드 업데이트
		void UpdateDashNode();

		// 대시하고 있는 상태인가
		bool _isDash;
		bool _isMoving;

		// 이동 때 사용하는 임시 변수들
		int _tempX;
		int _tempY;
		int _tempZ;

	public:
		// 플레이어 이동
		void MoveToTarget(DirectX::XMFLOAT3 targetPos, float speed);

		// 플레이어 대시
		void PlayerDash(DirectX::XMFLOAT3 targetPos, float speed);

		// 플레이어의 Y 좌표를 고정시키는 함수
		void SetPlayerY(float y);
		float GetPlayerY();

		// Navigation을 이용한 움직임
		void NavigationMove(float speed);
		void NavigationDash(float speed);
		std::vector<std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3>> _stopover;

		// 움직일 수 없는 공간을 클릭했을 때 가까운 지점으로 노드 재생성
		void RecalculateNavigation();

		/// 디버깅을 위한 함수
		void ShowPlayerInfo();
	};
}

