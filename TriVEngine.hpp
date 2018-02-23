#pragma once

#include "ITickable.hpp"
#include "IRenderable.hpp"
#include "EngineClock.hpp"
#include "IPhysicsTickable.hpp"
#include "PhysXErrorCallback.hpp"
#include "PhysXDefaultAllocator.hpp"

namespace TriV::Engine::Core
{
	static Engine::Physics::PhysXErrorCallback gDefaultErrorCallback;
	static Engine::Physics::PhysXDefaultAllocator gDefaultAllocatorCallback;

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

		void shutdownPhysics();
		void shutdownEngine();

		bool exitRequested = false;

		float frameDelta = 0;
		float timestep = 0;
		float framesPerSecond = 0;
		float frameAccumulator = 0;




		EngineClock engineClock;
	};

}
