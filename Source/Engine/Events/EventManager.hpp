#pragma once

#include <queue>
#include "BaseEngineEvent.hpp"
#include <memory>

namespace TriV::Engine::Core::Events
{
	class EventManager
	{
	private:
		std::queue<std::unique_ptr<BaseEngineEvent>> frameStartEvents;
		std::queue<std::unique_ptr<BaseEngineEvent>> afterPhysicsTickEvents;
		std::queue<std::unique_ptr<BaseEngineEvent>> afterTickEvents;
		std::queue<std::unique_ptr<BaseEngineEvent>> frameEndEvents;

		uint_fast64_t eventIDCounter = 1;
	public:
		void queueEvent(std::unique_ptr<BaseEngineEvent> evt);
		void dequeueEvent(const uint_fast64_t& eventID, const EventExecutionType& type = AfterTick);
		void processFrameStartEvents();
		void processAfterPhysicsTickEvents();
		void processAfterTickEvents();
		void processFrameEndEvents();

	};

}
