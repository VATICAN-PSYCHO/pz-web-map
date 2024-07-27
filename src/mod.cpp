#include "include/mod.hpp"

Mod::Mod(const std::string &id, const std::uint64_t &steamId,
		 const std::string &name, const std::string &pack,
		 const std::string &path)
	: id(id), steamId(steamId), name(name), pack(pack), path(path) {
	this->dependants = {};
	this->dependencies = {};
}

Mod::~Mod() {}

void Mod::addDependency(std::shared_ptr<Mod> mod) {
	dependencies.push_back(mod);
}

void Mod::addDependant(std::shared_ptr<Mod> mod) { dependants.push_back(mod); }

vector<std::shared_ptr<Mod>> Mod::getDependencies() const {
	return dependencies;
}

vector<std::shared_ptr<Mod>> Mod::getDependants() const { return dependants; }

vector<string> Mod::getRawDependencies() const { return rawDependencies; }

void Mod::setRawDependencies(const vector<string> rawDependencies) {
	this->rawDependencies = rawDependencies;
}

void Mod::setType(ModType type) { this->type = type; }

ModType Mod::getType() const { return type; }
