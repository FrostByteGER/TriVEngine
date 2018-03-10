#include "TriVWorld.hpp"
#include <iostream>

TriV::Engine::Core::TriVWorld::~TriVWorld()
{
}

void TriV::Engine::Core::TriVWorld::initWorld()
{
}

void TriV::Engine::Core::TriVWorld::tick()
{
	std::cout << "ENGINE::WORLD: Ticking!" << std::endl;
	for(auto& actor : actors)
	{
		actor->tick();
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
