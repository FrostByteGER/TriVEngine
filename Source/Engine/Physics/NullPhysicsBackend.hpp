#pragma once
#include "IPhysicsBackend.hpp"

namespace TriV::Engine::Physics
{
	class NullPhysicsBackend final : public IPhysicsBackend
	{
	public:
		void InitiatePhysicsBackend() override;
		void StepPhysics(const float& deltaTime) override;
		void DeinitiatePhysicsBackend() override;
	};
}
