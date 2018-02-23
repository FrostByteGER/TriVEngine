#pragma once
#include <pvd/PxPvd.h>
#include <memory>
#include <PxPhysics.h>
namespace TriV::Engine::Physics {

	class PhysicsEngine
	{
	public:


		PhysicsEngine();
		~PhysicsEngine();

		std::shared_ptr<physx::PxFoundation> foundation;

		std::shared_ptr<physx::PxPvd> pvdConnection;
		std::shared_ptr<physx::PxPvdTransport*> pvdTransport;

		std::shared_ptr<physx::PxPhysics> physics;


	};
}
