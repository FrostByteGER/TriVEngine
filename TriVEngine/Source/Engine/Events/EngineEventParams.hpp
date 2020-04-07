#pragma once
#include <any>

namespace TriV::Engine::Core::Events
{
	class EngineEventParams
	{

	private:
		std::any instigator;

	protected:
		explicit EngineEventParams(const std::any& instigator)
		{
			this->instigator = instigator;
		}
	};
}
