#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

#include "logger.hpp"

using std::string;
using std::vector;

struct Execution {
	bool parallel;
	uint32_t maxThreads;
};

struct ModConfig {
	bool enabled;
	std::vector<std::string> includeMods;
};

class Settings {
  private:
	Settings();
	~Settings();

  public:
	static Settings *getInstance();

	void loadSettings();

  private:
	static Settings *instance;
	Logger *logger;

  public:
	string gameDir;
	string workshopDir;
	string outputDir;
	vector<string> includePacksDir;
	Execution execution;
	ModConfig mods;
};

#endif
