#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <thread>

class Logger {
  private:
	Logger();
	~Logger();

  public:
	static Logger *getInstance();

	void setLevel(const std::string &level);

	void info(const std::string &message);
	void warn(const std::string &message);
	void error(const std::string &message);

  private:
	static Logger *instance;
};

#endif