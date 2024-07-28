#include "mod.hpp"

Mod::Mod(const std::string &id, const std::uint64_t &steamId,
		 const std::string &name, const std::string &pack,
		 const std::string &path) {
	this->id = id;
	this->steamId = steamId;
	this->name = name;
	this->pack = pack;
	this->path = path;
}

Mod::~Mod() {}

void Mod::addDependency(std::shared_ptr<Mod> mod) {
	this->dependencies.push_back(mod);
}

void Mod::addDependant(std::shared_ptr<Mod> mod) {
	this->dependants.push_back(mod);
}

std::vector<std::shared_ptr<Mod>> Mod::getDependencies() {
	return this->dependencies;
}

std::vector<std::shared_ptr<Mod>> Mod::getDependants() {
	return this->dependants;
}

std::vector<std::string> Mod::getRawDependencies() {
	return this->rawDependencies;
}

void Mod::setRawDependencies(const std::vector<std::string> rawDependencies) {
	this->rawDependencies = rawDependencies;
}

void Mod::setType(ModType type) { this->type = type; }

ModType Mod::getType() { return this->type; }

std::string Mod::getId() { return this->id; }

std::uint64_t Mod::getSteamId() { return this->steamId; }

std::string Mod::getName() { return this->name; }

std::string Mod::getPack() { return this->pack; }

std::string Mod::getPath() { return this->path; }
