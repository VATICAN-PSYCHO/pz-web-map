#include "logger.hpp"

#include <spdlog/spdlog.h>

Logger::Logger() {
	spdlog::set_pattern("[%H:%M:%S %z] [%^%l%$] [thread %t] %v");
	spdlog::set_level(spdlog::level::trace);
}

Logger::~Logger() { spdlog::shutdown(); }

Logger *Logger::getInstance() {
	if (!instance) {
		instance = new Logger();
	}

	return instance;
}

void Logger::info(const std::string &message) { spdlog::info(message); }

void Logger::warn(const std::string &message) { spdlog::warn(message); }

void Logger::error(const std::string &message) { spdlog::error(message); }

Logger *Logger::instance = nullptr;