#pragma once

#include <pvd/PxPvd.h>
#include <PxPhysics.h>
#include "PhysXErrorCallback.hpp"
#include "PhysXDefaultAllocator.hpp"
#include <extensions/PxDefaultCpuDispatcher.h>

namespace TriV::Engine::Physics 
{
	
	static PhysXErrorCallback gDefaultErrorCallback;
	static PhysXDefaultAllocator gDefaultAllocatorCallback;
	

	class PhysicsEngine
	{
	public:


		PhysicsEngine();
		~PhysicsEngine();

		void initiatePhysicsEngine();
		void stepPhysics(float deltaTime);
		void shutdownPhysicsEngine();

	private:
		
		physx::PxFoundation* foundation = nullptr;

		physx::PxPvd* pvdConnection = nullptr;
		physx::PxPvdTransport* pvdTransport = nullptr;

		physx::PxPhysics* physics = nullptr;
		physx::PxDefaultCpuDispatcher* dispatcher = nullptr;
		physx::PxScene* scene = nullptr;
		
		bool initiated = false;
		

	};
}
