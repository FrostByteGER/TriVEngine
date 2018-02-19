#include "TriVEngine.hpp"

void TriV::Engine::Core::TriVEngine::startEngine()
{
	initiateEngine();
}

void TriV::Engine::Core::TriVEngine::requestExit()
{
	exitRequested = true;
}

void TriV::Engine::Core::TriVEngine::initiateEngine()
{
	while(!exitRequested)
	{
		tickPhysics();
		tick();
		render();
	}
}

void TriV::Engine::Core::TriVEngine::shutdownEngine()
{
}

void TriV::Engine::Core::TriVEngine::tickPhysics()
{
}

void TriV::Engine::Core::TriVEngine::tick()
{
}

void TriV::Engine::Core::TriVEngine::render()
{
}
