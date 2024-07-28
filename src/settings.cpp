#include "settings.hpp"

using std::string;
using std::vector;

Settings::Settings() {
	gameDir = "";
	workshopDir = "";
	outputDir = "";
	includePacksDir = {};
	execution = {false, 0};
}
Settings::~Settings() {}

void Settings::loadSettings() {
	YAML::Node config = YAML::LoadFile("config.yaml");

	if (!config["game_dir"]) {
		printf("Game directory not found in config file.\n");
		exit(1);
	}

	if (!config["workshop_dir"]) {
		printf("Game mods directory not found in config file.\n");
		exit(1);
	}

	if (!config["output_dir"]) {
		printf("Output directory not found in config file.\n");
		exit(1);
	}

	if (!config["include_packs"]) {
		printf("Include packs not found in config file.\n");
		exit(1);
	}

	if (!config["include_packs"].IsSequence()) {
		printf("Include packs must be a list.\n");
		exit(1);
	}

	if (config["execution"]) {
		execution.parallel = config["execution"]["parallel"].as<bool>();
		execution.threads = config["execution"]["threads"].as<uint8_t>();
	}

	gameDir = config["game_dir"].as<string>();
	workshopDir = config["workshop_dir"].as<string>();
	outputDir = config["output_dir"].as<string>();
	includePacksDir = config["include_packs"].as<vector<string>>();
}

string Settings::getGameDir() { return gameDir; }

string Settings::steamWorkshopDir() { return workshopDir; }

string Settings::getOutputDir() { return outputDir; }

vector<string> Settings::getIncludePacksDir() { return includePacksDir; }

Settings *Settings::instance = nullptr;