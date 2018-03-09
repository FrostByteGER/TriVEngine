#include "EngineClock.hpp"

TriV::Engine::Core::EngineClock::EngineClock(): frameCount(0)
{
	lastEngineTime = engineClock.now();
	lastFrameTime = frameTimer.now();

	startPhysicsTime = physicsTimer.now();
	startRenderTime = renderTimer.now();
	startUpdateTime = updateTimer.now();

	endPhysicsTime = startPhysicsTime;
	endRenderTime = startRenderTime;
	endUpdateTime = startUpdateTime;

}

float TriV::Engine::Core::EngineClock::EngineElapsedSeconds()
{
	const std::chrono::time_point<std::chrono::high_resolution_clock> now = engineClock.now();
	const float delta = std::chrono::duration_cast<std::chrono::seconds>(now - lastEngineTime).count();
	lastFrameTime = now;
	return delta;
}

float TriV::Engine::Core::EngineClock::EngineElapsedMilliseconds()
{
	const std::chrono::time_point<std::chrono::high_resolution_clock> now = engineClock.now();
	const float delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastEngineTime).count();
	lastFrameTime = now;
	return delta;
}

float TriV::Engine::Core::EngineClock::EngineElapsedMicroseconds()
{
	const std::chrono::time_point<std::chrono::high_resolution_clock> now = engineClock.now();
	const float delta = std::chrono::duration_cast<std::chrono::microseconds>(now - lastEngineTime).count();
	lastFrameTime = now;
	return delta;
}

float TriV::Engine::Core::EngineClock::EngineElapsedNanoseconds()
{
	const std::chrono::time_point<std::chrono::high_resolution_clock> now = engineClock.now();
	const float delta = std::chrono::duration_cast<std::chrono::nanoseconds>(now - lastEngineTime).count();
	lastFrameTime = now;
	return delta;
}

float TriV::Engine::Core::EngineClock::PhysicsAverageSeconds() const
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(physicsTime).count() / frameCount;
}

float TriV::Engine::Core::EngineClock::UpdateAverageSeconds() const
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(updateTime).count() / frameCount;
}

float TriV::Engine::Core::EngineClock::RenderAverageSeconds() const
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(renderTime).count() / frameCount;
}

float TriV::Engine::Core::EngineClock::PhysicsAverage() const
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(physicsTime).count() / frameCount;
}

float TriV::Engine::Core::EngineClock::UpdateAverage() const
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(updateTime).count() / frameCount;
}

float TriV::Engine::Core::EngineClock::RenderAverage() const
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(renderTime).count() / frameCount;
}

void TriV::Engine::Core::EngineClock::StartPhysicsTimer()
{
	startPhysicsTime = physicsTimer.now();
}

void TriV::Engine::Core::EngineClock::StopPhysicsTimer()
{
	endPhysicsTime = physicsTimer.now();
	physicsTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endPhysicsTime - startPhysicsTime);
}

void TriV::Engine::Core::EngineClock::StartUpdateTimer()
{
	startUpdateTime = updateTimer.now();
}

void TriV::Engine::Core::EngineClock::StopUpdateTimer()
{
	endUpdateTime = updateTimer.now();
	updateTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endUpdateTime - startUpdateTime);
}

void TriV::Engine::Core::EngineClock::StartRenderTimer()
{
	startRenderTime = renderTimer.now();
}

void TriV::Engine::Core::EngineClock::StopRenderTimer()
{
	endRenderTime = renderTimer.now();
	renderTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endRenderTime - startRenderTime);
}

///
/// Returns Frame Delta in Milliseconds
///
float TriV::Engine::Core::EngineClock::GetFrameDelta()
{
	++frameCount;
	const std::chrono::time_point<std::chrono::high_resolution_clock> now = frameTimer.now();
	const float delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFrameTime).count();
	lastFrameTime = now;
	return delta;
}

///
/// Returns Frame Delta in Seconds
///
float TriV::Engine::Core::EngineClock::GetFrameDeltaSeconds()
{
	return GetFrameDelta() / 1000.0f;
}

uint64_t TriV::Engine::Core::EngineClock::GetFrameCount()
{
	return frameCount;
}

void TriV::Engine::Core::EngineClock::Reset()
{
	frameCount = 0;

	startPhysicsTime = physicsTimer.now();
	startRenderTime = renderTimer.now();
	startUpdateTime = updateTimer.now();

	endPhysicsTime = startPhysicsTime;
	endRenderTime = startRenderTime;
	endUpdateTime = startUpdateTime;

	physicsTime = std::chrono::duration<long long, std::ratio<1, 1000000000>>::zero();
	renderTime = std::chrono::duration<long long, std::ratio<1, 1000000000>>::zero();
	updateTime = std::chrono::duration<long long, std::ratio<1, 1000000000>>::zero();
}
