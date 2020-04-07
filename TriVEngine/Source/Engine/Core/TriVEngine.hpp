#pragma once

#include "ITickable.hpp"
#include "../Rendering/IRenderable.hpp"
#include "EngineClock.hpp"
#include "../Physics/IPhysicsTickable.hpp"
#include "../Physics/PhysicsEngine.hpp"
#include <vector>
#include <memory>
#include "TriVWorld.hpp"

namespace TriV::Engine::Core
{
	class TriVEngine : ITickable, IRenderable, Physics::IPhysicsTickable
	{
	public:
		void StartEngine();
		void RequestExit();



	private:
		void InitiateEngine();
		void InitiatePhysics();
		void InitiateEngineLoop();

		void TickPhysics() override;
		void Tick() override;
		void Render() override;

		void ShutdownWorlds();
		void ShutdownPhysics();
		void ShutdownEngine();

		bool _exitRequested = false;

		float _frameDelta = 0;
		float _timestep = 0;
		float _framesPerSecond = 0;
		float _frameAccumulator = 0;

		Engine::Physics::PhysicsEngine _physicsEngine;
		EngineClock _engineClock;

		std::vector<std::unique_ptr<TriVWorld>> _worlds;
	};

}
