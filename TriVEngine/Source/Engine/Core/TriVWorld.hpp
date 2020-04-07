#pragma once
#include <vector>
#include "TriVActor.hpp"
#include <memory>
#include "ITickable.hpp"
#include <string>


namespace TriV::Engine::Core
{
	class TriVWorld : protected ITickable
	{
	public:
		TriVWorld() = default;
		virtual ~TriVWorld();
		virtual void initWorld();
		void Tick() override;
		virtual void shutdownWorld();
	private:
		std::vector<std::unique_ptr<TriVActor>> actors;

		uint_fast64_t worldID = 0;
		std::string worldName;
		uint_fast64_t actorIDCounter = 0;

		void destroyActors();
		
	};

}
