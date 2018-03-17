#pragma once

#include <type_traits>
#include "EngineEventParams.hpp"
#include "EventExecutionType.hpp"
#include "BaseEngineEvent.hpp"

namespace TriV::Engine::Core::Events
{
	template<class T>
	class EngineEvent : public BaseEngineEvent
	{
		static_assert(std::is_same<EngineEventParams, T>::value || std::is_base_of<EngineEventParams, T>::value, "T must be a derivative of EngineEventParams!");

	private:
		T parameters;

	public:
		~EngineEvent() = default;

		EngineEvent(T parameters, EventExecutionType executionType) : BaseEngineEvent(executionType)
		{
			this->parameters = parameters;
		}

		EngineEvent(const EngineEvent& other)
			: BaseEngineEvent(other),
			parameters(other.parameters)
		{
		}

		EngineEvent(EngineEvent&& other) noexcept
			: BaseEngineEvent(std::move(other)),
			parameters(std::move(other.parameters))
		{
		}

		EngineEvent& operator=(const EngineEvent& other)
		{
			if (this == &other)
				return *this;
			BaseEngineEvent::operator =(other);
			parameters = other.parameters;
			return *this;
		}

		EngineEvent& operator=(EngineEvent&& other) noexcept
		{
			if (this == &other)
				return *this;
			BaseEngineEvent::operator =(std::move(other));
			parameters = std::move(other.parameters);
			return *this;
		}
	};
}