#include "TriVEngine.hpp"
#include "PxPhysicsAPI.h"
#include <iostream>

void TriV::Engine::Core::TriVEngine::startEngine()
{
	std::cout << "Starting TriVEngine..." << std::endl;
	initiateEngine();
}

void TriV::Engine::Core::TriVEngine::requestExit()
{
	exitRequested = true;
}

void TriV::Engine::Core::TriVEngine::initiateEngine()
{
	std::cout << "Initiating Engine..." << std::endl;
	initiatePhysics();
	initiateEngineLoop();
}

void TriV::Engine::Core::TriVEngine::initiatePhysics()
{
	std::cout << "Initiating Physics..." << std::endl;

	auto mFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback,
		gDefaultErrorCallback);
	if (!mFoundation)
		std::cout <<"PxCreateFoundation failed!" << std::endl;

	bool recordMemoryAllocations = true;
	
	
	auto mPvd = PxCreatePvd(*mFoundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("localhost", 5425, 100);
	bool connectionSuccess = mPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
	if (!connectionSuccess)
	{
		std::cout << "PVD connection failed!" << std::endl;
	}else
	{
		std::cout << "Connected to PhysX Visual Debugger instance! " << std::endl;
	}
		

	auto mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, physx::PxTolerancesScale(), recordMemoryAllocations, mPvd);
	if (!mPhysics)
	{
		std::cout << "PxCreatePhysics failed!" << std::endl;
	}else
	{
		std::cout << "Created PhysX object!" << std::endl;
	}
		
	
	/*
	 *if (!PxInitExtensions(*mPhysics, nullptr))
		std::cout << "PxInitExtensions failed!" << std::endl;
	*/
	
}

void TriV::Engine::Core::TriVEngine::initiateEngineLoop()
{
	std::cout << "Initiating Engine Loop..." << std::endl;
	while (!exitRequested)
	{
		tickPhysics();
		tick();
		render();
	}
}

void TriV::Engine::Core::TriVEngine::tickPhysics()
{
	//std::cout << "ENGINE::CORE::PHYSICS TICK" << std::endl;
}

void TriV::Engine::Core::TriVEngine::tick()
{
	//std::cout << "ENGINE::CORE::TICK" << std::endl;
}

void TriV::Engine::Core::TriVEngine::render()
{
	//std::cout << "ENGINE::CORE::RENDER" << std::endl;
}

void TriV::Engine::Core::TriVEngine::shutdownPhysics()
{
	std::cout << "Shutting down Physics..." << std::endl;
}

void TriV::Engine::Core::TriVEngine::shutdownEngine()
{
	std::cout << "Shutting down Engine..." << std::endl;
}
