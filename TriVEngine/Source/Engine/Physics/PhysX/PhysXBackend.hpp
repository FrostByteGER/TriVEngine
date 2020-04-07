#pragma once
#include <PxScene.h>
#include <extensions/PxDefaultCpuDispatcher.h>
#include <pvd/PxPvdTransport.h>
#include "PhysXDefaultAllocator.hpp"
#include "PhysXErrorCallback.hpp"
#include "../IPhysicsBackend.hpp"

namespace TriV::Engine::Physics::PhysX
{
	static PhysXErrorCallback GDefaultErrorCallback;
	static PhysXDefaultAllocator GDefaultAllocatorCallback;

	class PhysXBackend : public IPhysicsBackend
	{
	public:
		void InitiatePhysicsBackend() override;
		void DeinitiatePhysicsBackend() override;
		void StepPhysics(const float& deltaTime) override;
	private:
		physx::PxFoundation* _foundation = nullptr;
		physx::PxPvd* _pvdConnection = nullptr;
		physx::PxPvdTransport* _pvdTransport = nullptr;
		physx::PxPhysics* _physics = nullptr;
		physx::PxDefaultCpuDispatcher* _dispatcher = nullptr;
		physx::PxScene* _scene = nullptr;

		bool _initiated = false;
	};

}