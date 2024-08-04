#ifndef MOD_MANAGER_HPP
#define MOD_MANAGER_HPP

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

#include "config.hpp"
#include "logger.hpp"
#include "mod.hpp"
#include "texture_library.hpp"

class ModManager {
  public:
	ModManager(std::string path, TextureLibrary *textureLibrary);
	~ModManager();

	bool loadMods();

	void addMod(std::string id, std::shared_ptr<Mod> mod);
	auto getMods() { return mods; }

  private:
	Config *config = Config::getInstance();
	Logger *logger;
	std::string path;
	std::unordered_map<std::string, std::shared_ptr<Mod>> mods;
	TextureLibrary *textureLibrary;

	void setupDependencies();
	void removeOrphanedMods();
	void addTexturePacks(std::shared_ptr<Mod> mod);

	void processSteamWorkshopItem(std::string path);
	void processMod(std::string path, std::uint64_t steamId);
	bool isMapMod(std::string path);
	bool isTextureMod(std::string path);
	bool isNonEmptyExistsingDirectory(std::string path);
};

#endif