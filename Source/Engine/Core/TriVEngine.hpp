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
		void startEngine();
		void requestExit();



	private:
		void initiateEngine();
		void initiatePhysics();
		void initiateEngineLoop();

		void tickPhysics() override;
		void tick() override;
		void render() override;

		void shutdownWorlds();
		void shutdownPhysics();
		void shutdownEngine();

		bool exitRequested = false;

		float frameDelta = 0;
		float timestep = 0;
		float framesPerSecond = 0;
		float frameAccumulator = 0;

		Engine::Physics::PhysicsEngine physicsEngine;
		EngineClock engineClock;

		std::vector<std::unique_ptr<TriVWorld>> worlds;
	};

}
