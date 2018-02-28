#include "PhysicsEngine.hpp"
#include <iostream>
#include <pvd/PxPvdTransport.h>
#include <foundation/PxFoundationVersion.h>
#include <PxPhysicsVersion.h>
#include <common/PxTolerancesScale.h>
#include <PxSceneDesc.h>
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <PxScene.h>

TriV::Engine::Physics::PhysicsEngine::PhysicsEngine()
{
}

TriV::Engine::Physics::PhysicsEngine::~PhysicsEngine()
{
	std::cout << "PHYSICS: Destroying Physics..." << std::endl;
	shutdownPhysicsEngine();
}

void TriV::Engine::Physics::PhysicsEngine::initiatePhysicsEngine()
{
	std::cout << "PHYSICS: Initiating Physics..." << std::endl;

	foundation =  std::shared_ptr<physx::PxFoundation>(PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback));
	if (foundation)
		std::cout << "PHYSICS: PxCreateFoundation failed!" << std::endl;

	const bool recordMemoryAllocations = true;


	pvdConnection = std::shared_ptr<physx::PxPvd>(PxCreatePvd(*foundation));
	pvdTransport = std::shared_ptr<physx::PxPvdTransport>(physx::PxDefaultPvdSocketTransportCreate("localhost", 5425, 100));
	const bool connectionSuccess = pvdConnection->connect(*pvdTransport, physx::PxPvdInstrumentationFlag::eALL);
	if (!connectionSuccess)
	{
		std::cout << "PHYSICS: PVD connection failed!" << std::endl;
	}
	else
	{
		std::cout << "PHYSICS: Connected to PhysX Visual Debugger instance! " << std::endl;
	}


	physics = std::shared_ptr<physx::PxPhysics>(PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale(), recordMemoryAllocations, pvdConnection.get()));
	if (!physics)
	{
		std::cout << "PHYSICS: PxCreatePhysics failed!" << std::endl;
	}
	else
	{
		std::cout << "PHYSICS: Created PhysX object!" << std::endl;
	}

	physx::PxSceneDesc desc(physics->getTolerancesScale());
	desc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	dispatcher = std::shared_ptr<physx::PxDefaultCpuDispatcher>(physx::PxDefaultCpuDispatcherCreate(2));
	desc.cpuDispatcher = dispatcher.get();
	desc.filterShader = physx::PxDefaultSimulationFilterShader;

	scene = std::shared_ptr<physx::PxScene>(physics->createScene(desc));

	physx::PxPvdSceneClient* pvdClient = scene->getScenePvdClient();
	if(pvdClient)
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true); 
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}


	/*
	*if (!PxInitExtensions(*mPhysics, nullptr))
	std::cout << "PHYSICS: PxInitExtensions failed!" << std::endl;
	*/
}

void TriV::Engine::Physics::PhysicsEngine::shutdownPhysicsEngine()
{
	std::cout << "PHYSICS: shutting down Physics..." << std::endl;

	scene->release();
	scene = nullptr;
	dispatcher->release();
	dispatcher = nullptr;
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
