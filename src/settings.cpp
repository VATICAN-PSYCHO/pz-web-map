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

	this->logger = Logger::getInstance();

	if (!config) {
		this->logger->error("Config file not found.");
		exit(1);
	}

	auto logLevel = config["log_level"];
	this->logger->setLevel(logLevel.as<string>());

	if (!config["game_dir"]) {
		this->logger->error("Settings `game_dir` not found in config file.");
		exit(1);
	}

	if (!config["workshop_dir"]) {
		this->logger->error(
			"Settings `workshop_dir` not found in config file.");
		exit(1);
	}

	if (!config["output_dir"]) {
		this->logger->error("Settings `output_dir` not found in config file.");
		exit(1);
	}

	if (!config["include_packs"]) {
		this->logger->error(
			"Settings `include_packs` not found in config file.");
		exit(1);
	}

	if (!config["include_packs"].IsSequence()) {
		this->logger->error("Settings `include_packs` is not a sequence.");
		exit(1);
	}

	if (!config["execution"]) {
		this->logger->error("Settings `execution` not found in config file.");
		exit(1);
	}
	if (!config["execution"]["parallel"]) {
		this->logger->error(
			"Settings `execution`.`parallel` not found in config file.");
		exit(1);
	}
	if (!config["execution"]["threads"]) {
		this->logger->error(
			"Settings `execution`.`threads` not found in config file.");
		exit(1);
	}

	if (!config["mods"]) {
		this->logger->error("Settings `mods` not found in config file.");
		exit(1);
	}
	if (!config["mods"]["enabled"]) {
		this->logger->error(
			"Settings `mods`.`enabled` not found in config file.");
		exit(1);
	}
	if (!config["mods"]["include_mods"]) {
		this->logger->error(
			"Settings `mods`.`include_mods` not found in config file.");
		exit(1);
	}

	gameDir = config["game_dir"].as<string>();
	workshopDir = config["workshop_dir"].as<string>();
	outputDir = config["output_dir"].as<string>();
	includePacksDir = config["include_packs"].as<vector<string>>();

	execution.parallel = config["execution"]["parallel"].as<bool>();
	execution.threads = config["execution"]["threads"].as<uint32_t>();
	mods.enabled = config["mods"]["enabled"].as<bool>();
	mods.includeMods = config["mods"]["include_mods"].as<vector<string>>();

	this->logger->info("Settings loaded successfully.");
}

Settings *Settings::instance = nullptr;