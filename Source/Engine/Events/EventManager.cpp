#include "EventManager.hpp"

void TriV::Engine::Core::Events::EventManager::processFrameStartEvents()
{
	while(frameStartEvents.size() > 0)
	{
		frameStartEvents.front().executeEvent();
		frameStartEvents.pop();
	}
}

void TriV::Engine::Core::Events::EventManager::processAfterPhysicsTickEvents()
{
	while (afterPhysicsTickEvents.size() > 0)
	{
		afterPhysicsTickEvents.front().executeEvent();
		afterPhysicsTickEvents.pop();
	}
}

void TriV::Engine::Core::Events::EventManager::processAfterTickEvents()
{
	while (afterTickEvents.size() > 0)
	{
		afterTickEvents.front().executeEvent();
		afterTickEvents.pop();
	}
}

void TriV::Engine::Core::Events::EventManager::processFrameEndEvents()
{
	while (frameEndEvents.size() > 0)
	{
		frameEndEvents.front().executeEvent();
		frameEndEvents.pop();
	}
}
