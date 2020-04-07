#include "PhysXBackend.hpp"
#include <iostream>
#include <PxPhysics.h>
#include <PxPhysicsVersion.h>
#include <extensions/PxExtensionsAPI.h>

void TriV::Engine::Physics::PhysX::PhysXBackend::InitiatePhysicsBackend()
{
	_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, GDefaultAllocatorCallback, GDefaultErrorCallback);
	if (!_foundation)
		std::cout << "PHYSICS: PxCreateFoundation failed!" << std::endl;

	const bool recordMemoryAllocations = true;

	// We don't need a PVD connection in release mode as its deactivated in the release PhysX libs.
#ifdef _DEBUG
	_pvdConnection = PxCreatePvd(*_foundation);
	_pvdTransport = physx::PxDefaultPvdSocketTransportCreate("localhost", 5425, 100);
	const bool connectionSuccess = _pvdConnection->connect(*_pvdTransport, physx::PxPvdInstrumentationFlag::eALL);
	if (!connectionSuccess)
	{
		std::cout << "PHYSICS: PVD connection failed!" << std::endl;
	}
	else
	{
		std::cout << "PHYSICS: Connected to PhysX Visual Debugger instance! " << std::endl;
	}
#elif TriV_RELEASE
	_pvdConnection = nullptr;
#endif

	_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), recordMemoryAllocations, _pvdConnection);
	if (!_physics)
	{
		std::cout << "PHYSICS: PxCreatePhysics failed!" << std::endl;
	}
	else
	{
		std::cout << "PHYSICS: Created PhysX object!" << std::endl;
	}

	if (!PxInitExtensions(*_physics, _pvdConnection))
		std::cout << "PHYSICS: PxInitExtensions failed!" << std::endl;

	physx::PxSceneDesc desc(_physics->getTolerancesScale());
	desc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	desc.cpuDispatcher = _dispatcher;
	desc.filterShader = physx::PxDefaultSimulationFilterShader;

	_scene = _physics->createScene(desc);

	physx::PxPvdSceneClient* pvdClient = _scene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	/*
	auto material = physics->createMaterial(0.5f, 0.5f, 0.6f);

	physx::PxRigidStatic* groundPlane = physx::PxCreatePlane(*physics, physx::PxPlane(0, 1, 0, 0), *material);

	scene->addActor(*groundPlane);


	physx::PxShape* shape = physics->createShape(physx::PxBoxGeometry(2.0f, 2.0f, 2.0f), *material);
	auto stackZ = 10.0f;
	for (physx::PxU32 i = 0; i < 10; i++)
	{
		for (physx::PxU32 j = 0; j < 10 - i; j++)
		{
			physx::PxTransform localTm(physx::PxVec3(physx::PxReal(j * 2) - physx::PxReal(10 - i), physx::PxReal(i * 2 + 1), 0) * 50.0f);
			physx::PxRigidDynamic* body = physics->createRigidDynamic(physx::PxTransform(physx::PxVec3(0, 0, stackZ -= 10.0f)).transform(localTm));
			body->attachShape(*shape);
			physx::PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			scene->addActor(*body);
		}
	}
	shape->release();
	*/
}

void TriV::Engine::Physics::PhysX::PhysXBackend::DeinitiatePhysicsBackend()
{
	_scene->release();
	_scene = nullptr;
	_dispatcher->release();
	_dispatcher = nullptr;
	PxCloseExtensions();
	_physics->release();
	_physics = nullptr;

	// Kill Physics Visual Debugger connection if there is any.
	_pvdTransport->disconnect();
	_pvdConnection->disconnect();
	_pvdConnection->release();
	_pvdConnection = nullptr;
	_pvdTransport->release();
	_pvdTransport = nullptr;

	_foundation->release();
	_foundation = nullptr;
}

void TriV::Engine::Physics::PhysX::PhysXBackend::StepPhysics(const float& deltaTime)
{
	_scene->simulate(deltaTime);
	_scene->fetchResults(true);
}