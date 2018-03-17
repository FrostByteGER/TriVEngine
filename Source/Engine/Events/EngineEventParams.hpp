#pragma once
#include <any>

namespace TriV::Engine::Core::Events
{
	class EngineEventParams
	{

	private:
		std::any instigator;

	protected:
		EngineEventParams(std::any instigator)
		{
			this->instigator = instigator;
		}
	};
}
