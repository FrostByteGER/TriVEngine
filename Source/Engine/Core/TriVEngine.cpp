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

	worlds.push_back(std::make_unique<TriVWorld>());
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

		frameDelta = engineClock.GetFrameDeltaSeconds();
		frameAccumulator += frameDelta;
		if(frameAccumulator >= 1.0f)
		{
			framesPerSecond = engineClock.GetFrameCount() / frameAccumulator;
			std::cout << " FPS: " << framesPerSecond << " | Frame: " << frameDelta << "s | Render: " << engineClock.RenderAverage() << "ms | Update: " << engineClock.UpdateAverage() << "ms | Physics: " << engineClock.PhysicsAverage() << "ms" << std::endl;
			frameAccumulator = 0.0f;
			engineClock.Reset();
		}

		engineClock.StartPhysicsTimer();
		tickPhysics();
		engineClock.StopPhysicsTimer();

		engineClock.StartUpdateTimer();
		tick();
		engineClock.StopUpdateTimer();

		engineClock.StartRenderTimer();
		render();
		engineClock.StopRenderTimer();

		
	}
	shutdownEngine();
}

void TriV::Engine::Core::TriVEngine::tickPhysics()
{
	//std::cout << "ENGINE::CORE::PHYSICS TICK" << std::endl;
	physicsEngine.stepPhysics(frameDelta);
}

void TriV::Engine::Core::TriVEngine::tick()
{
	//std::cout << "ENGINE::CORE::TICK" << std::endl;
	for(auto& world : worlds)
	{
		world->tick();
	}
}

void TriV::Engine::Core::TriVEngine::render()
{
	//std::cout << "ENGINE::CORE::RENDER" << std::endl;
}

void TriV::Engine::Core::TriVEngine::shutdownWorlds()
{
	std::cout << "ENGINE: Shutting down Worlds..." << std::endl;
	worlds.clear();
}

void TriV::Engine::Core::TriVEngine::shutdownPhysics()
{
	std::cout << "ENGINE: Shutting down Physics..." << std::endl;
	physicsEngine.shutdownPhysicsEngine();
}

void TriV::Engine::Core::TriVEngine::shutdownEngine()
{
	std::cout << "ENGINE: Shutting down Engine..." << std::endl;
	shutdownWorlds();
	shutdownPhysics();
}
