#include "TriVEngine.hpp"
#include <iostream>
#include "../Utility/Logging/Log.hpp"

void TriV::Engine::Core::TriVEngine::StartEngine()
{
	TRIV_CORE_INFO("Starting TriVEngine...");
	InitiateEngine();
}

void TriV::Engine::Core::TriVEngine::RequestExit()
{
	_exitRequested = true;
}

void TriV::Engine::Core::TriVEngine::InitiateEngine()
{
	Utility::Logging::Log::Initialize();
	TRIV_CORE_INFO("Initiating Engine...");
	
	InitiatePhysics();

	_worlds.push_back(std::make_unique<TriVWorld>());
	InitiateEngineLoop();
}

void TriV::Engine::Core::TriVEngine::InitiatePhysics()
{
	TRIV_CORE_INFO("Initiating Physics...");

	_physicsEngine.InitiatePhysicsEngine();
	
}

void TriV::Engine::Core::TriVEngine::InitiateEngineLoop()
{
	TRIV_CORE_INFO("Initiating Engine Loop...");
	while (!_exitRequested)
	{

		_frameDelta = _engineClock.GetFrameDeltaSeconds();
		_frameAccumulator += _frameDelta;
		if(_frameAccumulator >= 1.0f)
		{
			_framesPerSecond = _engineClock.GetFrameCount() / _frameAccumulator;
			std::cout << " FPS: " << _framesPerSecond << " | Frame: " << _frameDelta << "s | Render: " << _engineClock.RenderAverage() << "ms | Update: " << _engineClock.UpdateAverage() << "ms | Physics: " << _engineClock.PhysicsAverage() << "ms" << std::endl;
			_frameAccumulator = 0.0f;
			_engineClock.Reset();
		}

		_engineClock.StartPhysicsTimer();
		TickPhysics();
		_engineClock.StopPhysicsTimer();

		_engineClock.StartUpdateTimer();
		Tick();
		_engineClock.StopUpdateTimer();

		_engineClock.StartRenderTimer();
		Render();
		_engineClock.StopRenderTimer();

		
	}
	ShutdownEngine();
}

void TriV::Engine::Core::TriVEngine::TickPhysics()
{
	_physicsEngine.StepPhysics(_frameDelta);
}

void TriV::Engine::Core::TriVEngine::Tick()
{
	for(auto& world : _worlds)
	{
		world->Tick();
	}
}

void TriV::Engine::Core::TriVEngine::Render()
{
}

void TriV::Engine::Core::TriVEngine::ShutdownWorlds()
{
	TRIV_CORE_INFO("Shutting down Worlds...");
	_worlds.clear();
}

void TriV::Engine::Core::TriVEngine::ShutdownPhysics()
{
	TRIV_CORE_INFO("Shutting down Physics...");
	_physicsEngine.ShutdownPhysicsEngine();
}

void TriV::Engine::Core::TriVEngine::ShutdownEngine()
{
	TRIV_CORE_INFO("Shutting down Engine...");
	ShutdownWorlds();
	ShutdownPhysics();
}
