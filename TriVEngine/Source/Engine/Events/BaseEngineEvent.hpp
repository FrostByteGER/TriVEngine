#pragma once
#include "EventExecutionType.hpp"
#include <iostream>

namespace TriV::Engine::Core::Events
{
	class BaseEngineEvent
	{

	private:
		EventExecutionType executionType;

	protected:
		explicit BaseEngineEvent(const EventExecutionType executionType)
		{
			this->executionType = executionType;
		}

	public:
		virtual ~BaseEngineEvent() = default;

		virtual void executeEvent() = 0;

		EventExecutionType getExecutionType() const
		{
			return executionType;
		}
	};
}
