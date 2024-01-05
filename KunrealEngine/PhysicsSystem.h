#pragma once

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

	class PhysicsSystem
	{
	private:
		PhysicsSystem();

		// ΩÃ±€≈Ê
	public:
		static PhysicsSystem& GetInstance()
		{
			static PhysicsSystem _instance;
			return _instance;
		}

	public:
		void Initialize();
		void Finalize();

	public:
		void CreatePhysXScene();
		physx::PxScene* GetPxScene();

	public:
		void CreateBoxCollider(GameObject* obj);

	private:
		void AddActorsToScene();

	private:
		std::vector<physx::PxRigidDynamic*> _rigidDynamics;
		std::vector<physx::PxRigidStatic*> _rigidStatics;
		std::vector<physx::PxFixedJoint*> _fixedJoints;

	private:
		physx::PxDefaultAllocator		_allocator;
		physx::PxDefaultErrorCallback	_errorCallback;
		physx::PxFoundation* _foundation;
		physx::PxPhysics* _physics;
		physx::PxDefaultCpuDispatcher* _dispatcher;
		physx::PxScene* _pxScene;
		physx::PxMaterial* _material;
		physx::PxPvd* _pvd;
	};
}