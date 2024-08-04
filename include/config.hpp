#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

#include "logger.hpp"

using std::string;
using std::vector;

struct Directories {
	string gameDir;
	string modsDir;
	string outputDir;
};

struct Execution {
	bool parallel;
	uint32_t threads;
};

struct Logging {
	string level;
	string file;
};

struct Mods {
	bool enabled;
	vector<string> include;
};

class Config {
  private:
	Config();
	~Config();

  public:
	static Config *getInstance();

	void load();

  private:
	static Config *instance;
	Logger *logger;

  public:
	Directories directories;
	Execution execution;
	Logging logging;
	Mods mods;
	vector<string> includePacksNames;
};

#endif
