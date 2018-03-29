#include "EventManager.hpp"

//TODO: FIX
void TriV::Engine::Core::Events::EventManager::queueEvent(const std::unique_ptr<BaseEngineEvent> evt)
{
	switch(evt->getExecutionType())
	{
	case FrameBeginning: 
		frameStartEvents.push(evt);
		break;
	case AfterPhysicsTick: 
		afterPhysicsTickEvents.push(evt);
		break;
	case AfterTick: 
		afterTickEvents.push(evt);
		break;
	case FrameEnd: 
		frameEndEvents.push(evt);
		break;
	default: 
		afterTickEvents.push(evt);
	}
}

void TriV::Engine::Core::Events::EventManager::processFrameStartEvents()
{
	while(frameStartEvents.size() > 0)
	{
		auto& evt = frameStartEvents.front();
		evt->executeEvent();
		frameStartEvents.pop();
	}
}

void TriV::Engine::Core::Events::EventManager::processAfterPhysicsTickEvents()
{
	while (afterPhysicsTickEvents.size() > 0)
	{
		auto& evt = afterPhysicsTickEvents.front();
		evt->executeEvent();
		afterPhysicsTickEvents.pop();
	}
}

void TriV::Engine::Core::Events::EventManager::processAfterTickEvents()
{
	while (afterTickEvents.size() > 0)
	{
		auto& evt = afterTickEvents.front();
		evt->executeEvent();
		afterTickEvents.pop();
	}
}

void TriV::Engine::Core::Events::EventManager::processFrameEndEvents()
{
	while (frameEndEvents.size() > 0)
	{
		auto& evt = frameEndEvents.front();
		evt->executeEvent();
		frameEndEvents.pop();
	}
}
