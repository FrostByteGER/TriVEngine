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

		EngineEvent(T parameters, const EventExecutionType executionType) : BaseEngineEvent(executionType)
		{
			this->parameters = parameters;
		}
	};
}