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

		uint64_t GetFrameCount();

		void Reset();

	private:
		uint64_t frameCount;

		std::chrono::high_resolution_clock engineClock;
		std::chrono::high_resolution_clock frameTimer;
		std::chrono::high_resolution_clock physicsTimer;
		std::chrono::high_resolution_clock updateTimer;
		std::chrono::high_resolution_clock renderTimer;

		std::chrono::time_point<std::chrono::high_resolution_clock> lastEngineTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameTime;

		std::chrono::time_point<std::chrono::high_resolution_clock> startPhysicsTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> startUpdateTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> startRenderTime;

		std::chrono::time_point<std::chrono::high_resolution_clock> endPhysicsTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> endUpdateTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> endRenderTime;

		std::chrono::duration<long long, std::ratio<1, 1000000000>> physicsTime;
		std::chrono::duration<long long, std::ratio<1, 1000000000>> updateTime;
		std::chrono::duration<long long, std::ratio<1, 1000000000>> renderTime;
	};
}
