#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

using std::string;
using std::vector;

struct Execution {
	bool parallel;
	uint8_t threads;
};

class Settings {
  public:
	Settings();
	~Settings();
	static Settings *getInstance() {
		if (instance == nullptr) {
			instance = new Settings();
		}
		return instance;
	}

	void loadSettings();
	string getGameDir();
	string steamWorkshopDir();
	vector<string> getIncludePacksDir();

	bool isParallel() { return execution.parallel; }
	uint8_t getThreads() { return execution.threads; }

  private:
	static Settings *instance;
	string gameDir;
	string workshopDir;
	vector<string> includePacksDir;
	Execution execution;
};

#endif
