#include "TriVActor.hpp"
#include <iostream>


TriV::Engine::Core::TriVActor::TriVActor()
{
}


TriV::Engine::Core::TriVActor::~TriVActor()
{
	std::cout << "Destroying Actor #" << actorID << std::endl;
}

void TriV::Engine::Core::TriVActor::Tick()
{
}
