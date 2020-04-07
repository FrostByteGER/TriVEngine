#include "TriVWorld.hpp"
#include <iostream>
#include "../Utility/Logging/Log.hpp"

TriV::Engine::Core::TriVWorld::~TriVWorld()
{
}

void TriV::Engine::Core::TriVWorld::initWorld()
{
}

void TriV::Engine::Core::TriVWorld::Tick()
{
	TRIV_CORE_INFO("ENGINE::WORLD: Ticking!");
	for(auto& actor : actors)
	{
		actor->Tick();
	}
}

void TriV::Engine::Core::TriVWorld::shutdownWorld()
{
	std::cout << "Shutting down World #" << worldID << " " << worldName.c_str() << std::endl;
	destroyActors();
}

void TriV::Engine::Core::TriVWorld::destroyActors()
{
	actors.clear();
}
