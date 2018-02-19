#pragma once
#include "ITickable.hpp"
#include "IRenderable.hpp"
#include "EngineClock.hpp"
#include "IPhysicsTickable.hpp"

namespace TriV::Engine::Core
{
	class TriVEngine : ITickable, IRenderable, Physics::IPhysicsTickable
	{
	public:
		void startEngine();
		void requestExit();



	private:
		void initiateEngine();
		void shutdownEngine();
		void tickPhysics() override;
		void tick() override;
		void render() override;

		bool exitRequested = false;

		EngineClock engineClock;
	};

}
