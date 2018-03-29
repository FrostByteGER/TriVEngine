#pragma once
#include <cstdint>
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
			revoked = false;
			eventID = 0;
			this->executionType = executionType;
		}

	public:
		virtual ~BaseEngineEvent() = default;

		virtual void executeEvent() = 0;

		bool revoked;

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

		EventExecutionType getExecutionType() const
		{
			return executionType;
		}

		uint_fast64_t getEventID() const
		{
			return eventID;
		}

		void setEventID(const uint_fast64_t id)
		{
			if(eventID > 0)
			{
				std::cout << "Warning: EventID has already been assigned, modifying after first assignment is not allowed!" << std::endl;
			}else
			{
				eventID = id;
			}
			
		}
	};
}
