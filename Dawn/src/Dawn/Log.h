#pragma once

#include "Dawn/Core.h"

#include <spdlog/spdlog.h>

namespace Dawn {

	class Log
	{
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return clientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;
	};

}

//Core Log Macros
#define DW_CORE_TRACE(...)   Dawn::Log::getCoreLogger()->trace(__VA_ARGS__)
#define DW_CORE_DEBUG(...)   Dawn::Log::getCoreLogger()->debug(__VA_ARGS__)
#define DW_CORE_INFO(...)    Dawn::Log::getCoreLogger()->info(__VA_ARGS__)
#define DW_CORE_WARN(...)    Dawn::Log::getCoreLogger()->warn(__VA_ARGS__)
#define DW_CORE_ERROR(...)   Dawn::Log::getCoreLogger()->error(__VA_ARGS__)
#define DW_CORE_FATAL(...)   Dawn::Log::getCoreLogger()->critical(__VA_ARGS__)

//Client Log Macros
#define DW_TRACE(...) Dawn::Log::getClientLogger()->trace(__VA_ARGS__)
#define DW_DEBUG(...) Dawn::Log::getClientLogger()->debug(__VA_ARGS__)
#define DW_INFO(...)  Dawn::Log::getClientLogger()->info(__VA_ARGS__)
#define DW_WARN(...)  Dawn::Log::getClientLogger()->warn(__VA_ARGS__)
#define DW_ERROR(...) Dawn::Log::getClientLogger()->error(__VA_ARGS__)
#define DW_FATAL(...) Dawn::Log::getClientLogger()->critical(__VA_ARGS__)