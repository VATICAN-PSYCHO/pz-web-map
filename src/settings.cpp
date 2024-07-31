#include "settings.hpp"
#include "logger.hpp"

using std::string;
using std::vector;

Settings::Settings() {}
Settings::~Settings() {}

Settings *Settings::getInstance() {
	if (!instance) {
		instance = new Settings();
	}

	return instance;
}

void Settings::loadSettings() {

	YAML::Node config = YAML::LoadFile("config.yaml");

	if (!config["game_dir"]) {
		this->logger->error("Game directory not found in config file.");
		exit(1);
	}

	if (!config["workshop_dir"]) {
		this->logger->error("Workshop directory not found in config file.");
		exit(1);
	}

	if (!config["output_dir"]) {
		this->logger->error("Output directory not found in config file.");
		exit(1);
	}

	if (!config["include_packs"]) {
		this->logger->error("Include packs not found in config file.");
		exit(1);
	}

	if (!config["include_packs"].IsSequence()) {
		this->logger->error("Include packs must be a list.");
		exit(1);
	}

	if (!config["execution"]) {
		this->logger->warn("Execution settings not found in config file.");
	}
	if (!config["execution"]["parallel"]) {
		this->logger->warn("Parallel execution not found in config file.");
	}
	if (!config["execution"]["max_threads"]) {
		this->logger->warn("Max threads not found in config file.");
	}

	if (!config["mods"]) {
		this->logger->warn("Mods settings not found in config file.");
	}
	if (!config["mods"]["enabled"]) {
		this->logger->warn("Mods enabled not found in config file.");
	}
	if (!config["mods"]["include_mods"]) {
		this->logger->warn("Include mods not found in config file.");
	}

	gameDir = config["game_dir"].as<string>();
	workshopDir = config["workshop_dir"].as<string>();
	outputDir = config["output_dir"].as<string>();
	includePacksDir = config["include_packs"].as<vector<string>>();

	execution.parallel = config["execution"]["parallel"].as<bool>();

	uint32_t realThreads = std::thread::hardware_concurrency();
	uint32_t configThreads = config["execution"]["max_threads"].as<uint32_t>();

	execution.maxThreads = std::clamp(configThreads, 1u, realThreads);

	mods.enabled = config["mods"]["enabled"].as<bool>();
	mods.includeMods = config["mods"]["include_mods"].as<vector<string>>();

	this->logger->info("Settings loaded successfully.");
}

Settings *Settings::instance = nullptr;