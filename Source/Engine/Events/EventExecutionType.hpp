#pragma once

namespace TriV::Engine::Core::Events
{
	enum EventExecutionType
	{
		FrameBeginning,
		AfterPhysicsTick,
		AfterTick,
		FrameEnd
	};
}