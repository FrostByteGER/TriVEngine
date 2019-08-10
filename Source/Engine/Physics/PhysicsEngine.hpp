#pragma once
#include "IPhysicsBackend.hpp"

namespace TriV::Engine::Physics 
{
	class PhysicsEngine
	{
	public:
		PhysicsEngine();
		~PhysicsEngine();

		void InitiatePhysicsEngine();
		void StepPhysics(const float& deltaTime);
		void ShutdownPhysicsEngine();

	private:
		IPhysicsBackend* _physicsBackend;
	};
}
