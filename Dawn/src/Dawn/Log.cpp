#include "dwpch.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Dawn {

	/*std::shared_ptr<spdlog::logger> Log::coreLogger;
	std::shared_ptr<spdlog::logger> Log::clientLogger;*/

	void Log::init()
	{
		spdlog::set_pattern("[%T][%^%4!l%$] %4n: %v");
		coreLogger = spdlog::stdout_color_mt("DAWN");
		coreLogger->set_level(spdlog::level::trace);

		clientLogger = spdlog::stdout_color_mt("APP");
		clientLogger->set_level(spdlog::level::trace);
	}

}