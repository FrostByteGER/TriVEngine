#pragma once

#include <chrono>

namespace TriV::Engine::Core
{
	class EngineClock
	{
	public:


		EngineClock();

		float EngineElapsedSeconds();
		float EngineElapsedMilliseconds();
		float EngineElapsedMicroseconds();
		float EngineElapsedNanoseconds();

		float PhysicsAverageSeconds() const;
		float UpdateAverageSeconds() const;
		float RenderAverageSeconds() const;
		float PhysicsAverage() const;
		float UpdateAverage() const;
		float RenderAverage() const;

		void StartPhysicsTimer();
		void StopPhysicsTimer();
		void StartUpdateTimer();
		void StopUpdateTimer();
		void StartRenderTimer();
		void StopRenderTimer();

		float GetFrameDelta();
		float GetFrameDeltaSeconds();

		uint64_t GetFrameCount() const;

		void Reset();

	private:
		uint64_t _frameCount {};

		std::chrono::high_resolution_clock _engineClock;
		std::chrono::high_resolution_clock _frameTimer;
		std::chrono::high_resolution_clock _physicsTimer;
		std::chrono::high_resolution_clock _updateTimer;
		std::chrono::high_resolution_clock _renderTimer;

		std::chrono::time_point<std::chrono::high_resolution_clock> _lastEngineTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> _lastFrameTime;

		std::chrono::time_point<std::chrono::high_resolution_clock> _startPhysicsTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> _startUpdateTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> _startRenderTime;

		std::chrono::time_point<std::chrono::high_resolution_clock> _endPhysicsTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> _endUpdateTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> _endRenderTime;

		std::chrono::duration<long long, std::ratio<1, 1000000000>> _physicsTime;
		std::chrono::duration<long long, std::ratio<1, 1000000000>> _updateTime;
		std::chrono::duration<long long, std::ratio<1, 1000000000>> _renderTime;
	};
}
