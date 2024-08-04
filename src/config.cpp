#include "config.hpp"
#include "logger.hpp"

using std::string;
using std::vector;

Config::Config() {
	this->logger = Logger::getInstance();
	this->directories = {};
	this->execution = {};
	this->logging = {};
	this->mods = {};
	this->includePacksNames = {};
}
Config::~Config() {}

Config *Config::getInstance() {
	if (!instance) {
		instance = new Config();
	}

	return instance;
}

void Config::load() {

	YAML::Node config = YAML::LoadFile("config.yaml");

	if (!config["dirs"]) {
		this->logger->error("Option `dirs` not found in config file.");
		exit(1);
	}

	if (!config["dirs"]["game"]) {
		this->logger->error("Option `dirs`.`game` not found in config file.");
		exit(1);
	}

	if (!config["dirs"]["mods"]) {
		this->logger->error("Option `dirs`.`mods` not found in config file.");
		exit(1);
	}

	if (!config["dirs"]["output"]) {
		this->logger->error("Option `dirs`.`output` not found in config file.");
		exit(1);
	}

	if (!config["execution"]) {
		this->logger->error("Option `execution` not found in config file.");
		exit(1);
	}
	if (!config["execution"]["parallel"]) {
		this->logger->error(
			"Option `execution`.`parallel` not found in config file.");
		exit(1);
	}
	if (!config["execution"]["threads"]) {
		this->logger->error(
			"Option `execution`.`threads` not found in config file.");
		exit(1);
	}

	if (!config["logging"]) {
		this->logger->error("Option `logging` not found in config file.");
		exit(1);
	}

	if (!config["logging"]["level"]) {
		this->logger->error(
			"Option `logging`.`level` not found in config file.");
		exit(1);
	}

	if (!config["logging"]["file"]) {
		this->logger->error(
			"Option `logging`.`file` not found in config file.");
		exit(1);
	}

	if (!config["mods"]) {
		this->logger->error("Option `mods` not found in config file.");
		exit(1);
	}

	if (!config["mods"]["enabled"]) {
		this->logger->error(
			"Option `mods`.`enabled` not found in config file.");
		exit(1);
	}

	if (!config["mods"]["include"]) {
		this->logger->error(
			"Option `mods`.`include` not found in config file.");
		exit(1);
	}

	if (!config["include_packs"]) {
		this->logger->error("Option `include_packs` not found in config file.");
		exit(1);
	}

	this->directories.gameDir = config["dirs"]["game"].as<string>();
	this->directories.modsDir = config["dirs"]["mods"].as<string>();
	this->directories.outputDir = config["dirs"]["output"].as<string>();

	this->execution.parallel = config["execution"]["parallel"].as<bool>();
	this->execution.threads = config["execution"]["threads"].as<uint32_t>();

	this->logging.level = config["logging"]["level"].as<string>();
	this->logging.file = config["logging"]["file"].as<string>();

	this->mods.enabled = config["mods"]["enabled"].as<bool>();
	this->mods.include = config["mods"]["include"].as<vector<string>>();

	this->includePacksNames = config["include_packs"].as<vector<string>>();

	this->logger->info("Config file loaded successfully.");
	this->logger->setLevel(this->logging.level);
}

Config *Config::instance = nullptr;