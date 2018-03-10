#include "PhysicsEngine.hpp"
#include <iostream>

#include <pvd/PxPvdTransport.h>
#include <foundation/PxFoundationVersion.h>
#include <PxPhysicsVersion.h>
#include <common/PxTolerancesScale.h>
#include <PxSceneDesc.h>
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <PxScene.h>
#include <extensions/PxExtensionsAPI.h>
#include <PxRigidDynamic.h>
#include <PxRigidStatic.h>


TriV::Engine::Physics::PhysicsEngine::PhysicsEngine() 
{
}

TriV::Engine::Physics::PhysicsEngine::~PhysicsEngine()
{
	std::cout << "PHYSICS: Destroying..." << std::endl;
}

void TriV::Engine::Physics::PhysicsEngine::initiatePhysicsEngine()
{
	std::cout << "PHYSICS: Initiating..." << std::endl;
	
	foundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	if (!foundation)
		std::cout << "PHYSICS: PxCreateFoundation failed!" << std::endl;

	const bool recordMemoryAllocations = true;
	

	
	pvdConnection = PxCreatePvd(*foundation);
	pvdTransport = physx::PxDefaultPvdSocketTransportCreate("localhost", 5425, 100);
	const bool connectionSuccess = pvdConnection->connect(*pvdTransport, physx::PxPvdInstrumentationFlag::eALL);
	if (!connectionSuccess)
	{
		std::cout << "PHYSICS: PVD connection failed!" << std::endl;
	}
	else
	{
		std::cout << "PHYSICS: Connected to PhysX Visual Debugger instance! " << std::endl;
	}

	
	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale(), recordMemoryAllocations, pvdConnection);
	if (!physics)
	{
		std::cout << "PHYSICS: PxCreatePhysics failed!" << std::endl;
	}
	else
	{
		std::cout << "PHYSICS: Created PhysX object!" << std::endl;
	}

	if (!PxInitExtensions(*physics, pvdConnection))
		std::cout << "PHYSICS: PxInitExtensions failed!" << std::endl;

	physx::PxSceneDesc desc(physics->getTolerancesScale());
	desc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	desc.cpuDispatcher = dispatcher;
	desc.filterShader = physx::PxDefaultSimulationFilterShader;
	
	scene = physics->createScene(desc);

	physx::PxPvdSceneClient* pvdClient = scene->getScenePvdClient();
	if(pvdClient)
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

void TriV::Engine::Physics::PhysicsEngine::stepPhysics(const float deltaTime)
{
	scene->simulate(deltaTime);
	scene->fetchResults(true);
}

void TriV::Engine::Physics::PhysicsEngine::shutdownPhysicsEngine()
{
	std::cout << "PHYSICS: shutting down..." << std::endl;
	
	scene->release();
	scene = nullptr;
	dispatcher->release();
	dispatcher = nullptr;
	PxCloseExtensions();
	physics->release();
	physics = nullptr;

	// Kill Physics Visual Debugger connection if there is any.
	pvdTransport->disconnect();
	pvdConnection->disconnect();
	pvdConnection->release();
	pvdConnection = nullptr;
	pvdTransport->release();
	pvdTransport = nullptr;

	foundation->release();
	foundation = nullptr;
	
}
