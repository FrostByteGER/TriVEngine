#pragma once


namespace TriV::Engine::Core::Physics
{
	class IPhysicsTickable
	{
	public:
		virtual ~IPhysicsTickable() = default;
		virtual void TickPhysics() = 0;
	};
}