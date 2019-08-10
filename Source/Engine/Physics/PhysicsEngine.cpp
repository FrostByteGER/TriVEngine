#include "PhysicsEngine.hpp"
#include <iostream>
#include "NullPhysicsBackend.hpp"

#ifdef USE_PHYSX_BACKEND
#include "PhysX/PhysXBackend.hpp"
#endif


TriV::Engine::Physics::PhysicsEngine::PhysicsEngine() 
{
}

TriV::Engine::Physics::PhysicsEngine::~PhysicsEngine()
{
	std::cout << "PHYSICS: Destroying..." << std::endl;
}

void TriV::Engine::Physics::PhysicsEngine::InitiatePhysicsEngine()
{
	std::cout << "PHYSICS: Initiating..." << std::endl;
	
#ifdef USE_PHYSX_BACKEND
	_physicsBackend = new PhysX::PhysXBackend();
#else
	_physicsBackend = new NullPhysicsBackend();
#endif
	
	if (_physicsBackend)
		_physicsBackend->InitiatePhysicsBackend();

	

}

void TriV::Engine::Physics::PhysicsEngine::StepPhysics(const float& deltaTime)
{
	if (_physicsBackend)
		_physicsBackend->StepPhysics(deltaTime);
}

void TriV::Engine::Physics::PhysicsEngine::ShutdownPhysicsEngine()
{
	std::cout << "PHYSICS: shutting down..." << std::endl;
	if (_physicsBackend)
		_physicsBackend->InitiatePhysicsBackend();
	
}
