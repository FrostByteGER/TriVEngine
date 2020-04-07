#pragma once

#include <memory>
#include <string>

//#include "spdlog/spdlog.h"

namespace TriV::Engine::Utility::Logging
{
	class Log
	{
	public:
		static void Initialize();

		//static std::shared_ptr<spdlog::logger> GetCoreLogger() { return _coreLogger; }
		//static std::shared_ptr<spdlog::logger> GetClientLogger() { return _clientLogger; }

		static std::shared_ptr<std::string> GetCoreLogger() { return _coreLogger; }
		static std::shared_ptr<std::string> GetClientLogger() { return _clientLogger; }

	private:
		//static std::shared_ptr<spdlog::logger> _coreLogger;
		//static std::shared_ptr<spdlog::logger> _clientLogger;
		
		static std::shared_ptr<std::string> _coreLogger;
		static std::shared_ptr<std::string> _clientLogger;
	};

}

// Engine Logging
#define TRIV_CORE_FATAL(...) //::TriV::Engine::Utility::Logging::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define TRIV_CORE_ERROR(...) //::TriV::Engine::Utility::Logging::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TRIV_CORE_WARN(...) //::TriV::Engine::Utility::Logging::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TRIV_CORE_INFO(...) //::TriV::Engine::Utility::Logging::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TRIV_CORE_TRACE(...) //::TriV::Engine::Utility::Logging::Log::GetCoreLogger()->trace(__VA_ARGS__)

//Client Logging
#define TRIV_FATAL(...) //::TriV::Engine::Utility::Logging::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define TRIV_ERROR(...) //::TriV::Engine::Utility::Logging::Log::GetClientLogger()->error(__VA_ARGS__)
#define TRIV_WARN(...) //::TriV::Engine::Utility::Logging::Log::GetClientLogger()->warn(__VA_ARGS__)
#define TRIV_INFO(...) //::TriV::Engine::Utility::Logging::Log::GetClientLogger()->info(__VA_ARGS__)
#define TRIV_TRACE(...) //::TriV::Engine::Utility::Logging::Log::GetClientLogger()->trace(__VA_ARGS__)