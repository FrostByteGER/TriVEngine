#pragma once
#include <pvd/PxPvd.h>
#include <memory>
#include <PxPhysics.h>
#include "PhysXErrorCallback.hpp"
#include "PhysXDefaultAllocator.hpp"
#include <extensions/PxDefaultCpuDispatcher.h>

namespace TriV::Engine::Physics {

	class PhysicsEngine
	{
	public:


		PhysicsEngine();
		~PhysicsEngine();

		void initiatePhysicsEngine();
		void shutdownPhysicsEngine();

		std::shared_ptr<physx::PxFoundation> foundation;

		std::shared_ptr<physx::PxPvd> pvdConnection;
		std::shared_ptr<physx::PxPvdTransport> pvdTransport;

		std::shared_ptr<physx::PxPhysics> physics;
		std::shared_ptr<physx::PxDefaultCpuDispatcher> dispatcher;
		std::shared_ptr<physx::PxScene> scene;

		bool initiated = false;
		
		static PhysXErrorCallback gDefaultErrorCallback;
		static PhysXDefaultAllocator gDefaultAllocatorCallback;
	};
}
