#include "EngineClock.hpp"

TriV::Engine::Core::EngineClock::EngineClock()
{
	_lastEngineTime = _engineClock.now();
	_lastFrameTime = _frameTimer.now();

	_startPhysicsTime = _physicsTimer.now();
	_startRenderTime = _renderTimer.now();
	_startUpdateTime = _updateTimer.now();

	_endPhysicsTime = _startPhysicsTime;
	_endRenderTime = _startRenderTime;
	_endUpdateTime = _startUpdateTime;
}

float TriV::Engine::Core::EngineClock::EngineElapsedSeconds()
{
	const std::chrono::time_point<std::chrono::high_resolution_clock> now = _engineClock.now();
	const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now - _lastEngineTime);
	const float delta = std::chrono::duration_cast<std::chrono::duration<float>>(seconds).count();
	_lastFrameTime = now;
	return delta;
}

float TriV::Engine::Core::EngineClock::EngineElapsedMilliseconds()
{
	const std::chrono::time_point<std::chrono::high_resolution_clock> now = _engineClock.now();
	const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastEngineTime);
	const float delta = std::chrono::duration_cast<std::chrono::duration<float>>(milliseconds).count();
	_lastFrameTime = now;
	return delta;
}

float TriV::Engine::Core::EngineClock::EngineElapsedMicroseconds()
{
	const std::chrono::time_point<std::chrono::high_resolution_clock> now = _engineClock.now();
	const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(now - _lastEngineTime);
	const float delta = std::chrono::duration_cast<std::chrono::duration<float>>(microseconds).count();
	_lastFrameTime = now;
	return delta;
}

float TriV::Engine::Core::EngineClock::EngineElapsedNanoseconds()
{
	const std::chrono::time_point<std::chrono::high_resolution_clock> now = _engineClock.now();
	const auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(now - _lastEngineTime);
	const float delta = std::chrono::duration_cast<std::chrono::duration<float>>(nanoseconds).count();
	_lastFrameTime = now;
	return delta;
}

float TriV::Engine::Core::EngineClock::PhysicsAverageSeconds() const
{
	return std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::duration_cast<std::chrono::milliseconds>(_physicsTime)).count() / _frameCount;
}

float TriV::Engine::Core::EngineClock::UpdateAverageSeconds() const
{
	return std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::duration_cast<std::chrono::milliseconds>(_updateTime)).count() / _frameCount;
}

float TriV::Engine::Core::EngineClock::RenderAverageSeconds() const
{
	return std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::duration_cast<std::chrono::milliseconds>(_renderTime)).count() / _frameCount;
}

float TriV::Engine::Core::EngineClock::PhysicsAverage() const
{
	return std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::duration_cast<std::chrono::milliseconds>(_physicsTime)).count() / _frameCount;
}

float TriV::Engine::Core::EngineClock::UpdateAverage() const
{
	return std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::duration_cast<std::chrono::milliseconds>(_updateTime)).count() / _frameCount;
}

float TriV::Engine::Core::EngineClock::RenderAverage() const
{
	return std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::duration_cast<std::chrono::milliseconds>(_renderTime)).count() / _frameCount;
}

void TriV::Engine::Core::EngineClock::StartPhysicsTimer()
{
	_startPhysicsTime = _physicsTimer.now();
}

void TriV::Engine::Core::EngineClock::StopPhysicsTimer()
{
	_endPhysicsTime = _physicsTimer.now();
	_physicsTime = std::chrono::duration_cast<std::chrono::nanoseconds>(_endPhysicsTime - _startPhysicsTime);
}

void TriV::Engine::Core::EngineClock::StartUpdateTimer()
{
	_startUpdateTime = _updateTimer.now();
}

void TriV::Engine::Core::EngineClock::StopUpdateTimer()
{
	_endUpdateTime = _updateTimer.now();
	_updateTime = std::chrono::duration_cast<std::chrono::nanoseconds>(_endUpdateTime - _startUpdateTime);
}

void TriV::Engine::Core::EngineClock::StartRenderTimer()
{
	_startRenderTime = _renderTimer.now();
}

void TriV::Engine::Core::EngineClock::StopRenderTimer()
{
	_endRenderTime = _renderTimer.now();
	_renderTime = std::chrono::duration_cast<std::chrono::nanoseconds>(_endRenderTime - _startRenderTime);
}

///
/// Returns Frame Delta in Milliseconds
///
float TriV::Engine::Core::EngineClock::GetFrameDelta()
{
	++_frameCount;
	const std::chrono::time_point<std::chrono::high_resolution_clock> now = _frameTimer.now();
	const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFrameTime);
	const float delta = std::chrono::duration_cast<std::chrono::duration<float>>(milliseconds).count();
	_lastFrameTime = now;
	return delta;
}

///
/// Returns Frame Delta in Seconds
///
float TriV::Engine::Core::EngineClock::GetFrameDeltaSeconds()
{
	return GetFrameDelta() / 1000.0f;
}

uint64_t TriV::Engine::Core::EngineClock::GetFrameCount() const
{
	return _frameCount;
}

void TriV::Engine::Core::EngineClock::Reset()
{
	_frameCount = 0;

	_startPhysicsTime = _physicsTimer.now();
	_startRenderTime = _renderTimer.now();
	_startUpdateTime = _updateTimer.now();

	_endPhysicsTime = _startPhysicsTime;
	_endRenderTime = _startRenderTime;
	_endUpdateTime = _startUpdateTime;

	_physicsTime = std::chrono::duration<long long, std::ratio<1, 1000000000>>::zero();
	_renderTime = std::chrono::duration<long long, std::ratio<1, 1000000000>>::zero();
	_updateTime = std::chrono::duration<long long, std::ratio<1, 1000000000>>::zero();
}
