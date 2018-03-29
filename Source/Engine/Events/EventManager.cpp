#include "EventManager.hpp"

//TODO: FIX
void TriV::Engine::Core::Events::EventManager::queueEvent(const std::unique_ptr<BaseEngineEvent> evt)
{
	evt->setEventID(eventIDCounter);
	++eventIDCounter;

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

void TriV::Engine::Core::Events::EventManager::dequeueEvent(const uint_fast64_t& eventID, const EventExecutionType& type)
{
	switch (type)
	{
	case FrameBeginning:
		//frameStartEvents.push(evt);
		break;
	case AfterPhysicsTick:
		//this->afterPhysicsTickEvents.push(evt);
		break;
	case AfterTick:
		//this->afterTickEvents.push(evt);
		break;
	case FrameEnd:
		//this->frameEndEvents.push(evt);
		break;
	default:
		break;
		//this->afterTickEvents.push(evt);
	}
}

void TriV::Engine::Core::Events::EventManager::processFrameStartEvents()
{
	while(frameStartEvents.size() > 0)
	{
		auto& evt = frameStartEvents.front();
		if (!evt->revoked) 
			evt->executeEvent();
		frameStartEvents.pop();
	}
}

void TriV::Engine::Core::Events::EventManager::processAfterPhysicsTickEvents()
{
	while (afterPhysicsTickEvents.size() > 0)
	{
		auto& evt = afterPhysicsTickEvents.front();
		if (!evt->revoked)
			evt->executeEvent();
		afterPhysicsTickEvents.pop();
	}
}

void TriV::Engine::Core::Events::EventManager::processAfterTickEvents()
{
	while (afterTickEvents.size() > 0)
	{
		auto& evt = afterTickEvents.front();
		if (!evt->revoked)
			evt->executeEvent();
		afterTickEvents.pop();
	}
}

void TriV::Engine::Core::Events::EventManager::processFrameEndEvents()
{
	while (frameEndEvents.size() > 0)
	{
		auto& evt = frameEndEvents.front();
		if (!evt->revoked)
			evt->executeEvent();
		frameEndEvents.pop();
	}
}
