#include "EventManager.hpp"

void TriV::Engine::Core::Events::EventManager::queueEvent(std::unique_ptr<BaseEngineEvent> evt)
{
	switch (evt->getExecutionType())
	{
	case FrameBeginning:
		frameStartEvents.push(std::move(evt));
		break;
	case AfterPhysicsTick:
		afterPhysicsTickEvents.push(std::move(evt));
		break;
	case AfterTick:
		afterTickEvents.push(std::move(evt));
		break;
	case FrameEnd:
		frameEndEvents.push(std::move(evt));
		break;
	default:
		afterTickEvents.push(std::move(evt));
	}
}

void TriV::Engine::Core::Events::EventManager::processFrameStartEvents()
{
	while(frameStartEvents.size() > 0)
	{
		auto evt = std::move(frameStartEvents.front());
		evt->executeEvent();
		frameStartEvents.pop();
	}
}

void TriV::Engine::Core::Events::EventManager::processAfterPhysicsTickEvents()
{
	while (afterPhysicsTickEvents.size() > 0)
	{
		auto evt = std::move(afterPhysicsTickEvents.front());
		evt->executeEvent();
		afterPhysicsTickEvents.pop();
	}
}

void TriV::Engine::Core::Events::EventManager::processAfterTickEvents()
{
	while (afterTickEvents.size() > 0)
	{
		auto evt = std::move(afterTickEvents.front());
		evt->executeEvent();
		afterTickEvents.pop();
	}
}

void TriV::Engine::Core::Events::EventManager::processFrameEndEvents()
{
	while (frameEndEvents.size() > 0)
	{
		auto evt = std::move(frameEndEvents.front());
		evt->executeEvent();
		frameEndEvents.pop();
	}
}
