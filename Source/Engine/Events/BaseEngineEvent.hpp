#pragma once
#include <cstdint>
#include "EventExecutionType.hpp"

namespace TriV::Engine::Core::Events
{
	class BaseEngineEvent
	{

	private:
		EventExecutionType executionType;

	protected:

		explicit BaseEngineEvent(const EventExecutionType executionType)
		{
			revoked = false;
			eventID = 0;
			this->executionType = executionType;
		}

	public:
		virtual ~BaseEngineEvent() = default;

		virtual void executeEvent() = 0;

		bool revoked;
		uint_fast64_t eventID;


		BaseEngineEvent(const BaseEngineEvent& other) = default;

		BaseEngineEvent(BaseEngineEvent&& other) noexcept
			: executionType(other.executionType),
			revoked(other.revoked),
			eventID(other.eventID)
		{
		}

		BaseEngineEvent& operator=(const BaseEngineEvent& other)
		{
			if (this == &other)
				return *this;
			executionType = other.executionType;
			revoked = other.revoked;
			eventID = other.eventID;
			return *this;
		}

		BaseEngineEvent& operator=(BaseEngineEvent&& other) noexcept
		{
			if (this == &other)
				return *this;
			executionType = other.executionType;
			revoked = other.revoked;
			eventID = other.eventID;
			return *this;
		}
	};
}
