#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <thread>

class Logger {
  public:
	Logger(const Logger &) = delete;
	Logger &operator=(const Logger &) = delete;

	static Logger *instance() {
		static Logger instance;
		return &instance;
	}

	void info(const std::string &message);
	void warn(const std::string &message);
	void error(const std::string &message);

  private:
	Logger();
	~Logger();
};

#endif