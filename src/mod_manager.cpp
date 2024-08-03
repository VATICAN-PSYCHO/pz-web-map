#include "mod_manager.hpp"
#include "file_system.hpp"
#include "logger.hpp"
#include "settings.hpp"
#include "thread_pool.hpp"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <thread>

using std::string;
using std::vector;

ModManager::ModManager() {
	this->logger = Logger::getInstance();
	this->logger->info("ModManager instance created.");
}

ModManager::~ModManager() {}

void ModManager::addMod(string id, std::shared_ptr<Mod> mod) { mods[id] = mod; }

void ModManager::loadMods(string path) {
	if (!settings->mods.enabled) {
		this->logger->info("Mods are disabled.");
		return;
	}

	auto steamWorkshopDir = std::filesystem::path(path);

	std::vector<std::filesystem::path> workshopItems;

	for (const auto &workshopItem :
		 std::filesystem::directory_iterator(steamWorkshopDir)) {
		workshopItems.push_back(workshopItem.path());
	}

	for (const auto &workshopItem : workshopItems) {
		this->processSteamWorkshopItem(workshopItem);
	}

	this->setupDependencies();

	for (auto it = mods.begin(); it != mods.end();) {
		auto mod = it->second;
		if (mod->getType() == ModType::TEXTURE &&
			mod->getDependants().empty()) {
			it = mods.erase(it);
		} else {
			++it;
		}
	}
}

void ModManager::processSteamWorkshopItem(string path) {
	auto modsDir = std::filesystem::path(path) / "mods";
	auto pos = path.find_last_of("/");

	std::uint64_t steamId = 0;

	if (pos != string::npos) {
		auto workshopId = path.substr(pos + 1);
		steamId = std::stoull(workshopId);
	}

	for (const auto &modEntry : std::filesystem::directory_iterator(modsDir)) {

		if (!modEntry.is_directory()) {
			continue;
		}

		this->processMod(modEntry.path(), steamId);
	}
}

void ModManager::processMod(string path, std::uint64_t steamId) {
	auto modDirectory = std::filesystem::path(path);

	auto isMapMod = this->isMapMod(modDirectory);
	auto isTextureMod = this->isTextureMod(modDirectory);

	if (!isMapMod && !isTextureMod) {
		return;
	}

	auto modInfoFile = modDirectory / "mod.info";

	if (!FileSystem::validatePath(modInfoFile)) {

		return;
	}

	if (!FileSystem::isFile(modInfoFile)) {
		return;
	}

	std::ifstream modInfoFileStream(modInfoFile);

	string line, id, name, deps, pack;
	while (std::getline(modInfoFileStream, line)) {
		if (!line.empty() && line.back() == '\r') {
			line.pop_back();
		}

		if (line.find("id=", 0) == 0) {
			id = line.substr(line.find("=") + 1);
		} else if (line.find("name=", 0) == 0) {
			name = line.substr(line.find("=") + 1);
		} else if (line.find("require=", 0) == 0) {
			deps = line.substr(line.find("=") + 1);
		} else if (line.find("pack=", 0) == 0) {
			pack = line.substr(line.find("=") + 1);
		}
	}

	vector<string> dependencies;
	std::size_t pos = 0;
	while ((pos = deps.find(",")) != string::npos) {
		string dependency = deps.substr(0, pos);
		dependencies.push_back(dependency);
		deps.erase(0, pos + 1);
	}
	dependencies.push_back(deps);

	auto mod = std::make_shared<Mod>(id, steamId, name, pack, path);

	mod->setRawDependencies(dependencies);

	if (isMapMod && isTextureMod) {
		mod->setType(ModType::MIXED);
	} else if (isMapMod) {
		mod->setType(ModType::MAP);
	} else if (isTextureMod) {
		mod->setType(ModType::TEXTURE);
	}

	this->addMod(id, mod);
}

void ModManager::setupDependencies() {
	for (const auto &mod : mods) {
		auto modId = mod.first;
		auto modPtr = mod.second;

		for (const auto &dependency : modPtr->getRawDependencies()) {
			if (mods.find(dependency) != mods.end()) {
				auto depMod = mods[dependency];
				modPtr->addDependency(depMod);
				depMod->addDependant(modPtr);
			}
		}
	}
}

bool ModManager::isMapMod(string path) {
	auto modDirectory = std::filesystem::path(path);

	auto mapsDirectory = modDirectory / "media" / "maps";

	return this->isNonEmptyExistsingDirectory(mapsDirectory);
}

bool ModManager::isTextureMod(string path) {
	auto modDirectory = std::filesystem::path(path);

	auto packsDirectory = modDirectory / "media" / "texturepacks";

	return this->isNonEmptyExistsingDirectory(packsDirectory);
}

bool ModManager::isNonEmptyExistsingDirectory(string path) {

	auto directory = std::filesystem::path(path);

	auto exists = FileSystem::validatePath(directory);
	if (!exists)
		return false;

	auto isDirectory = FileSystem::isDirectory(directory);

	if (!isDirectory)
		return false;

	auto isNonEmpty = FileSystem::isNonEmptyDirectory(directory);

	return isNonEmpty;
}