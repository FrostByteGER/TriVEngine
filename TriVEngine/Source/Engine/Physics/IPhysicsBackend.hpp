#pragma once
namespace TriV::Engine::Physics
{
	class IPhysicsBackend
	{
	public:
		virtual ~IPhysicsBackend() = default;
		virtual void InitiatePhysicsBackend() = 0;
		virtual void StepPhysics(const float& deltaTime) = 0;
		virtual void DeinitiatePhysicsBackend() = 0;
	};

}
