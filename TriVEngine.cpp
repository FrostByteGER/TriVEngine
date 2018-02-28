#include "TriVEngine.hpp"
#include <iostream>

void TriV::Engine::Core::TriVEngine::startEngine()
{
	std::cout << "ENGINE: Starting TriVEngine..." << std::endl;
	initiateEngine();
}

void TriV::Engine::Core::TriVEngine::requestExit()
{
	exitRequested = true;
}

void TriV::Engine::Core::TriVEngine::initiateEngine()
{
	std::cout << "ENGINE: Initiating Engine..." << std::endl;
	initiatePhysics();
	initiateEngineLoop();
}

void TriV::Engine::Core::TriVEngine::initiatePhysics()
{
	std::cout << "ENGINE: Initiating Physics..." << std::endl;

	physicsEngine.initiatePhysicsEngine();
	
}

void TriV::Engine::Core::TriVEngine::initiateEngineLoop()
{
	std::cout << "ENGINE: Initiating Engine Loop..." << std::endl;
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
	std::cout << "ENGINE: Shutting down Physics..." << std::endl;
	physicsEngine.shutdownPhysicsEngine();
}

void TriV::Engine::Core::TriVEngine::shutdownEngine()
{
	std::cout << "ENGINE: Shutting down Engine..." << std::endl;
	shutdownPhysics();
}
