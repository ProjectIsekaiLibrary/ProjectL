#pragma once

#include <unordered_map>
#include "CommonHeader.h"
#include "../KunrealMath/MathHeaders.h"
#pragma comment(lib,"../Bin/x64/Debug/KunrealMath.lib")

#pragma warning(disable : 26451 26495 6297 33010)
#include <physx/PxPhysics.h>
#include <physx/PxPhysicsAPI.h>
#include <physx/extensions/PxDefaultAllocator.h>
#include <physx/extensions/PxDefaultErrorCallback.h>
#pragma warning(default : 26451 26495 6297 33010)

namespace KunrealEngine
{
	class GameObject;
	class BoxCollider;

	class _DECLSPEC PhysicsSystem : public physx::PxSimulationEventCallback
	{
		// actor의 userData에 추가할 구조체
		struct PhysicsUserData
		{
			GameObject* gameObject;
			physx::PxShape* shape;
		};

	private:
		PhysicsSystem();

		// 싱글톤
	public:
		static PhysicsSystem& GetInstance()
		{
			static PhysicsSystem _instance;
			return _instance;
		}

	public:
		void Initialize();
		void Release();
		void FixedUpdate();

	public:
		// eventcallback
		virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
		virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
		virtual void onConstraintBreak(physx::PxConstraintInfo*, physx::PxU32) override;
		virtual void onWake(physx::PxActor**, physx::PxU32) override;
		virtual void onSleep(physx::PxActor**, physx::PxU32) override;
		virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform * poseBuffer, const physx::PxU32 count) override;

	public:
		void CreatePhysXScene();
		physx::PxScene* GetPxScene();

	public:
		// 움직이는 객체에 부여할 box collider
		void CreateDynamicBoxCollider(BoxCollider* collider);

		// 움직이지 않는 객체에 부여할 box collider
		void CreateStaticBoxCollider(BoxCollider* collider);

		// sphere collider
		void CreateDynamicSphereCollider();

		// physx 객체의 크기를 BoxCollider의 크기에 맞게
		void SetBoxSize(BoxCollider* collider);

		// Dynamic actor들 업데이트
		void UpdateDynamics();
		
		// Static actor들 업데이트
		void UpdateStatics();

		void TestFunc();			/// 실험용 꼭 지워

	private:
		std::vector<physx::PxRigidDynamic*>	_rigidDynamics;
		std::vector<physx::PxRigidStatic*>	_rigidStatics;
		std::vector<physx::PxFixedJoint*>	_fixedJoints;

		// 컴포넌트에서 가지고 있는 Transform 정보를 지속적으로 넘겨주기 위해
		// actor의 userData 기능을 쓰면 될 것 같기도한데.. 일단 테스트중
		std::unordered_map<BoxCollider*, physx::PxRigidDynamic*> _dynamicMap;
		std::unordered_map<BoxCollider*, physx::PxRigidStatic*> _staticMap;

	private:
		physx::PxDefaultAllocator			_allocator;
		physx::PxDefaultErrorCallback		_errorCallback;
		physx::PxTolerancesScale			_torlerance;
		physx::PxFoundation*				_foundation;
		physx::PxPhysics*					_physics;
		physx::PxScene*						_pxScene;
		physx::PxDefaultCpuDispatcher*		_dispatcher;
		physx::PxMaterial*					_material;
		physx::PxPvd*						_pvd;


	// eventcallback 대응함수
	private:
		bool IsTrigger(const physx::PxFilterData& data);

		static physx::PxFilterFlags TriggerUsingFilterShader(physx::PxFilterObjectAttributes /*attributes0*/, physx::PxFilterData filterData0,
			physx::PxFilterObjectAttributes /*attributes1*/, physx::PxFilterData filterData1,
			physx::PxPairFlags& pairFlags, const void* /*constantBlock*/, physx::PxU32 /*constantBlockSize*/);
	
	private:
		// eventcallback으로부터 받은 actor가 어떤 collider에 대응하는지
		BoxCollider* GetColliderFromDynamic(const physx::PxRigidDynamic* dynamic);
	};
}