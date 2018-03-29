#pragma once
#include "EventExecutionType.hpp"
#include <iostream>

namespace TriV::Engine::Core::Events
{
	class BaseEngineEvent
	{

	private:
		EventExecutionType executionType;
		uint_fast64_t eventID;

	protected:
		explicit BaseEngineEvent(const EventExecutionType executionType)
		{
			this->executionType = executionType;
		}

	public:
		virtual ~BaseEngineEvent() = default;

		virtual void executeEvent() = 0;

		BaseEngineEvent(const BaseEngineEvent& other) = default;

		BaseEngineEvent(BaseEngineEvent&& other) noexcept
			: executionType(other.executionType)
		{
		}

		BaseEngineEvent& operator=(const BaseEngineEvent& other)
		{
			if (this == &other)
				return *this;
			executionType = other.executionType;
			return *this;
		}

		BaseEngineEvent& operator=(BaseEngineEvent&& other) noexcept
		{
			if (this == &other)
				return *this;
			executionType = other.executionType;
			return *this;
		}

		EventExecutionType getExecutionType() const
		{
			return executionType;
		}
	};
}
