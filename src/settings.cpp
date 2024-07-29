#include "settings.hpp"
#include "logger.hpp"

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
	auto logger = Logger::instance();
	YAML::Node config = YAML::LoadFile("config.yaml");

	if (!config["game_dir"]) {
		logger->error("Game directory not found in config file.");
		exit(1);
	}

	if (!config["workshop_dir"]) {
		logger->error("Workshop directory not found in config file.");
		exit(1);
	}

	if (!config["output_dir"]) {
		logger->error("Output directory not found in config file.");
		exit(1);
	}

	if (!config["include_packs"]) {
		logger->error("Include packs not found in config file.");
		exit(1);
	}

	if (!config["include_packs"].IsSequence()) {
		logger->error("Include packs must be a list.");
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

	logger->info("Settings loaded successfully.");
}

string Settings::getGameDir() { return gameDir; }

string Settings::steamWorkshopDir() { return workshopDir; }

string Settings::getOutputDir() { return outputDir; }

vector<string> Settings::getIncludePacksDir() { return includePacksDir; }

Settings *Settings::instance = nullptr;