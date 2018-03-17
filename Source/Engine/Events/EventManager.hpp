#pragma once
#include <queue>
#include "BaseEngineEvent.hpp"

namespace TriV::Engine::Core::Events
{
	class EventManager
	{
	private:
		std::queue<BaseEngineEvent> frameStartEvents;
		std::queue<BaseEngineEvent> afterPhysicsTickEvents;
		std::queue<BaseEngineEvent> afterTickEvents;
		std::queue<BaseEngineEvent> frameEndEvents;

		void processFrameStartEvents();
		void processAfterPhysicsTickEvents();
		void processAfterTickEvents();
		void processFrameEndEvents();

	};

}
