#include <cassert>
#include "PhysicsSystem.h"
#include "TimeManager.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "Transform.h"

KunrealEngine::PhysicsSystem::PhysicsSystem()
	:_foundation(nullptr), _physics(nullptr), _dispatcher(nullptr), _pxScene(nullptr),
	_material(nullptr), _pvd(nullptr)
{

}

void KunrealEngine::PhysicsSystem::Initialize()
{
	_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);

	// visual debugger 세팅, 로컬에 연결
	_pvd = PxCreatePvd(*_foundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

	// 버전, 세팅, 단위 등의 정보를 담은 물리						// _tolerance를 따로 정해주지 않았다면
	_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, _pvd);

	PxInitExtensions(*_physics, _pvd);

	// 물리 씬을 생성
	CreatePhysXScene();

	// 머티리얼 생성(임의)	/// 이게 0.5라서?
	_material = _physics->createMaterial(0.5f, 0.5f, 0.5f);

}

void KunrealEngine::PhysicsSystem::Release()
{
	/// actor들 release 하는 부분 필요

	_rigidDynamics.clear();
	_rigidStatics.clear();
	_dynamicMap.clear();
	_staticMap.clear();

	_pxScene->release();
	_dispatcher->release();
	_material->release();
	_pvd->release();
	_physics->release();
	_foundation->release();
}

void KunrealEngine::PhysicsSystem::FixedUpdate()
{
	UpdateDynamics();
	UpdateStatics();

	_pxScene->simulate(TimeManager::GetInstance().GetDeltaTime());
	_pxScene->fetchResults(true);

}

void KunrealEngine::PhysicsSystem::CreatePhysXScene()
{
	// 씬에 대한 설정		// SceneDescription
	physx::PxSceneDesc sceneDesc(_physics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.f, 0.f, 0.f);				// 중력 설정		// 중력이 없으니 0 0 0
	_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = _dispatcher;
	sceneDesc.simulationEventCallback = this;

	// 필터를 어떻게 넣어주느냐에 따라 이벤트를 callback 받을 때 다른 행동
	//sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	sceneDesc.filterShader = TriggerUsingFilterShader;

	_pxScene = _physics->createScene(sceneDesc);

	// Pvd에 정보 보내기
	physx::PxPvdSceneClient* pvdClient = _pxScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	// scene에 필터 정보 전달
	
}

physx::PxScene* KunrealEngine::PhysicsSystem::GetPxScene()
{
	return _pxScene;
}

void KunrealEngine::PhysicsSystem::CreateDynamicBoxCollider(BoxCollider* collider)
{
	/// 그가 남긴 코드
	///// Collider 및 BoxCollider 컴포넌트 생성하는 부분 필요
	//
	////physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(box->GetWidth() / 2, box->GetHeight() / 2, box->GetDepth() / 2), *_material);
	//
	//KunrealMath::Float3 pos = obj->GetComponent<Transform>()->GetPosition();
	//physx::PxTransform localTm(physx::PxVec3(pos.x, pos.y, pos.z));
	//physx::PxRigidDynamic* body = _physics->createRigidDynamic(localTm);
	////body->attachShape(*shape);
	//body->setAngularDamping(0.5f);
	//body->setLinearDamping(5.0f); 
	//
	////if (box->gameObject->objName == "player")
	//{
	//	body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
	//	body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
	//	body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
	//}
	////body->setMass(4.5f);
	////physx::PxRigidBodyExt::updateMassAndInertia(*body, 1.0f);
	////_scene->addActor(*body);
	//
	////box->SetPhysXRigid(body);
	////body->userData = box;
	//_rigidDynamics.push_back(body);
	//
	////shape->release();

	physx::PxTransform trans(physx::PxVec3(collider->GetColliderPos().x, collider->GetColliderPos().y, collider->GetColliderPos().z));
	physx::PxBoxGeometry geometry(physx::PxVec3(0.5f, 0.5f, 0.5f));

	physx::PxRigidDynamic* boxActor = _physics->createRigidDynamic(trans);

	physx::PxShape* boxShape = _physics->createShape(physx::PxBoxGeometry(0.5f, 0.5f, 0.5f), *_material);

	boxActor->attachShape(*boxShape);

	// 우리 게임에서는 정교한 물리가 필요하지 않고 트리거 발동 여부만 필요
	// 충돌 시 아무런 물리적 변화가 없도록 설정
	physx::PxRigidDynamicLockFlags flag =
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z |
		physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X |
		physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y |
		physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z;

	boxActor->setRigidDynamicLockFlags(flag);

	// UserData
	PhysicsUserData data(collider->GetOwner(), boxShape);
	boxActor->userData = &data;

	_pxScene->addActor(*boxActor);
	_dynamicMap.insert(std::make_pair(collider, boxActor));
	_rigidDynamics.push_back(boxActor);
}

void KunrealEngine::PhysicsSystem::CreateStaticBoxCollider(BoxCollider* collider)
{
	physx::PxTransform trans(physx::PxVec3(collider->GetColliderPos().x, collider->GetColliderPos().y, collider->GetColliderPos().z));
	physx::PxRigidStatic* boxActor = _physics->createRigidStatic(trans);

	physx::PxBoxGeometry geometry(physx::PxVec3(1.f, 1.f, 1.f));

	_pxScene->addActor(*boxActor);
	_staticMap.insert(std::make_pair(collider, boxActor));
	_rigidStatics.push_back(boxActor);
}

void KunrealEngine::PhysicsSystem::UpdateDynamics()
{
	// 포지션 관련
	for (const auto& pair : _dynamicMap)
	{
		pair.second->setGlobalPose(physx::PxTransform(physx::PxVec3(pair.first->GetColliderPos().x, pair.first->GetColliderPos().y, pair.first->GetColliderPos().z)));
	}
}

void KunrealEngine::PhysicsSystem::UpdateStatics()
{
	// 포지션 관련
	for (const auto& pair : _staticMap)
	{
		pair.second->setGlobalPose(physx::PxTransform(physx::PxVec3(pair.first->GetColliderPos().x, pair.first->GetColliderPos().y, pair.first->GetColliderPos().z)));
	}
}

void KunrealEngine::PhysicsSystem::SetBoxSize(BoxCollider* collider)
{
	/// attach된 shape의 크기를 직접 변경해주는 함수가 없어서 사이즈 변경 함수가 호출될 때마다 삭제/추가를 반복하도록 만들었음
	// 붙여줬던 shape를 떼주고
	_dynamicMap.at(collider)->detachShape(*static_cast<PhysicsUserData*>(_dynamicMap.at(collider)->userData)->shape);

	// 메모리 해제
	/// detachShape에서 delete까지 해주는 모양이다
	//static_cast<PhysicsUserData*>(_dynamicMap.at(collider)->userData)->shape->release();

	// 크기에 맞게 새로운 shape 생성
	physx::PxShape* boxShape = _physics->createShape(physx::PxBoxGeometry(
		collider->GetBoxSize().x / 2.f, 
		collider->GetBoxSize().y / 2.f, 
		collider->GetBoxSize().z / 2.f), *_material);

	// 새롭게 만든 shape 추가
	_dynamicMap.at(collider)->attachShape(*boxShape);

	// userData 업데이트
	static_cast<PhysicsUserData*>(_dynamicMap.at(collider)->userData)->shape = boxShape;
}

void KunrealEngine::PhysicsSystem::TestFunc()
{
	physx::PxTransform transform(physx::PxVec3(0.0f, 10.0f, 0.0f));		// 위치
	physx::PxBoxGeometry geometry(physx::PxVec3(2.0f, 2.0f, 2.0f));		// 크기

	physx::PxTransform transform2(physx::PxVec3(1.0f, 10.0f, 0.0f));
	physx::PxBoxGeometry geometry2(physx::PxVec3(2.0f, 2.0f, 2.0f));

	physx::PxRigidDynamic* boxActor = PxCreateDynamic(*_physics, transform, geometry, *_material, 1.0f);
	physx::PxRigidDynamic* boxActor2 = PxCreateDynamic(*_physics, transform2, geometry2, *_material, 1.0f);

	//physx::PxShape* shape = _physics->createShape;

	//boxActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);

	physx::PxRigidDynamicLockFlags flag =
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z |
		physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X |
		physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y |
		physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z;

	//boxActor->setRigidDynamicLockFlags(flag);

	//shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	//shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);

	//boxActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);		// 중력의 영향을 받지 않게
	//boxActor->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, true);	// simlation을 돌리지 않게 -> sleep 상태로 만들어서 아무것도 안하게 만듦

	//boxActor->userData = this;

	//_pxScene->addActor(*boxActor);
	//_pxScene->addActor(*boxActor2);
}

bool KunrealEngine::PhysicsSystem::IsTrigger(const physx::PxFilterData& data)
{
	if (data.word0 != 0xffffffff)
		return false;
	if (data.word1 != 0xffffffff)
		return false;
	if (data.word2 != 0xffffffff)
		return false;
	if (data.word3 != 0xffffffff)
		return false;
	return true;
}

physx::PxFilterFlags KunrealEngine::PhysicsSystem::TriggerUsingFilterShader(physx::PxFilterObjectAttributes /*attributes0*/, physx::PxFilterData filterData0, physx::PxFilterObjectAttributes /*attributes1*/, physx::PxFilterData filterData1, physx::PxPairFlags& pairFlags, const void* /*constantBlock*/, physx::PxU32 /*constantBlockSize*/)
{
	pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;

	return physx::PxFilterFlag::eDEFAULT;
}

KunrealEngine::BoxCollider* KunrealEngine::PhysicsSystem::GetColliderFromDynamic(const physx::PxRigidDynamic* dynamic)
{
	for (const auto& pair : _dynamicMap)
	{
		if (pair.second == dynamic)
		{
			return pair.first;
		}
	}
}

void KunrealEngine::PhysicsSystem::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
{
	
}

void KunrealEngine::PhysicsSystem::onWake(physx::PxActor**, physx::PxU32)
{
	
}

void KunrealEngine::PhysicsSystem::onSleep(physx::PxActor** actors, physx::PxU32 count)
{
	
}

void KunrealEngine::PhysicsSystem::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
	const physx::PxContactPair& current = *pairs++;

	/// collider를 여러개 더 추가해봐도 pairHeader의 actor들은 두개로 잡힌다
	/// 충돌한 두개만을 받아오는건 맞다는 이야기

	/// PxActor와 PxRigidDynamic과의 관계 때문에?
	/// PxBase -> PxActor -> PxRigidActor -> PxRigidBody -> PxRigidDynamic 순으로 상속 됌
	/// pairHeader가 들고 있는 멤버는 PxActor
									/// 다운 캐스팅이니까 dynamic_cast는 하지마라
	//physx::PxRigidActor* casted1 = static_cast<physx::PxRigidActor*>(pairHeader.actors[0]);
	//physx::PxRigidBody* casted2 = static_cast<physx::PxRigidBody*>(casted1);
	//physx::PxRigidDynamic* casted3 = static_cast<physx::PxRigidDynamic*>(casted2);

	//auto finalData = casted3->userData;
	//PhysicsUserData* finalDatata = static_cast<PhysicsUserData*>(casted3->userData);

	/// void*가 문제인걸까 결국 userData에는 nullptr만이 들어있을뿐이다
	/// 모르겠다

	/// cast는 안되는거 같다
	//physx::PxRigidDynamic* asfgd = static_cast<physx::PxRigidDynamic*>(pairHeader.actors[1]);
	//physx::PxTransform ads = asfgd->getGlobalPose();

	/// PxActor의 userData라 안되는 모양이다		// 부모의 기능 같은데 왜 접근이 안될까
	/// PxRigidDynamic으로 만들고 addActor 함수로 추가했는데 왜 안되는지는 의문
	//PhysicsUserData* dataA = static_cast<PhysicsUserData*>(pairHeader.actors[0]->userData);
	//PhysicsUserData* dataB = static_cast<PhysicsUserData*>(pairHeader.actors[1]->userData);

	/// 결국 collider에 직접 접근해서 데이터를 넣어주기로함
	/// 나도 이쁘게 만들고 싶었어
	physx::PxRigidDynamic* casted1 = static_cast<physx::PxRigidDynamic*>(pairHeader.actors[0]);
	physx::PxRigidDynamic* casted2 = static_cast<physx::PxRigidDynamic*>(pairHeader.actors[1]);

	BoxCollider* col1 = GetColliderFromDynamic(casted1);
	BoxCollider* col2 = GetColliderFromDynamic(casted2);

	// 충돌이 발생했을 때
	if (current.events & (physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_CCD))
	{
		// 충돌 여부를 true로
		col1->_isCollided = true;
		col2->_isCollided = true;

		// 상대 오브젝트에 대한 정보를 넘겨줌
		col1->_targetObj = col2->GetOwner();
		col2->_targetObj = col1->GetOwner();	
	}

	// 충돌에서 벗어났을 때
	/// 근데 크기가 안 맞네
	if (current.events & (physx::PxPairFlag::eNOTIFY_TOUCH_LOST))
	{
		// 충돌 여부를 false로
		col1->_isCollided = false;
		col2->_isCollided = false;

		// 충돌에서 벗어났으니 nullptr로
		col1->_targetObj = nullptr;
		col2->_targetObj = nullptr;

		int a = 10;
	}
}

void KunrealEngine::PhysicsSystem::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
	
}

void KunrealEngine::PhysicsSystem::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
{
	
}