#pragma once

#include <queue>
#include "BaseEngineEvent.hpp"
#include <memory>

namespace TriV::Engine::Core::Events
{
	class EventManager
	{
	public:
		void queueEvent(std::unique_ptr<BaseEngineEvent> evt);
		void processFrameStartEvents();
		void processAfterPhysicsTickEvents();
		void processAfterTickEvents();
		void processFrameEndEvents();

	private:
		std::queue<std::unique_ptr<BaseEngineEvent>> frameStartEvents;
		std::queue<std::unique_ptr<BaseEngineEvent>> afterPhysicsTickEvents;
		std::queue<std::unique_ptr<BaseEngineEvent>> afterTickEvents;
		std::queue<std::unique_ptr<BaseEngineEvent>> frameEndEvents;
	};

}
