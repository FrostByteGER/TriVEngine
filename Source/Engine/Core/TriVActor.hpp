#pragma once
#include "ITickable.hpp"
#include <cstdint>

namespace TriV::Engine::Core
{
	class TriVActor : public ITickable
	{
	public:
		TriVActor();
		virtual ~TriVActor();
		void tick() override;
		uint_fast64_t actorID = 0;
	};
}
