#include "logger.hpp"

#include <spdlog/spdlog.h>

Logger::Logger() {
	spdlog::set_pattern("[%H:%M:%S %z] [%^%l%$] [thread %t] %v");
	spdlog::set_level(spdlog::level::info);
}

Logger::~Logger() { spdlog::shutdown(); }

Logger *Logger::getInstance() {
	if (!instance) {
		instance = new Logger();
	}

	return instance;
}

void Logger::setLevel(const std::string &level) {

	if (level == "trace") {
		spdlog::set_level(spdlog::level::trace);
	} else if (level == "debug") {
		spdlog::set_level(spdlog::level::debug);
	} else if (level == "info") {
		spdlog::set_level(spdlog::level::info);
	} else if (level == "warn") {
		spdlog::set_level(spdlog::level::warn);
	} else if (level == "error") {
		spdlog::set_level(spdlog::level::err);
	} else if (level == "critical") {
		spdlog::set_level(spdlog::level::critical);
	} else {
		spdlog::warn("Invalid log level: {}", level);
		spdlog::warn("Setting log level to `info`");
	}
}

void Logger::info(const std::string &message) { spdlog::info(message); }

void Logger::warn(const std::string &message) { spdlog::warn(message); }

void Logger::error(const std::string &message) { spdlog::error(message); }

Logger *Logger::instance = nullptr;