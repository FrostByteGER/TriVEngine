#include "Log.hpp"
//#include "spdlog/sinks/stdout_color_sinks.h"

namespace TriV::Engine::Utility::Logging
{
	//std::shared_ptr<spdlog::logger> Log::_coreLogger;
	//std::shared_ptr<spdlog::logger> Log::_clientLogger;

	std::shared_ptr<std::string> Log::_coreLogger;
	std::shared_ptr<std::string> Log::_clientLogger;

	void Log::Initialize()
	{
		/*
		spdlog::set_pattern("%^[%T] %n: %v%$");
		_coreLogger = spdlog::stdout_color_mt("TriVEngine");
		_coreLogger->set_level(spdlog::level::trace);
		_clientLogger = spdlog::stdout_color_mt("Application");
		_clientLogger->set_level(spdlog::level::trace);

		*/
	}
}
