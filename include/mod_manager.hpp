#ifndef MOD_MANAGER_HPP
#define MOD_MANAGER_HPP

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

#include "logger.hpp"
#include "mod.hpp"
#include "settings.hpp"
#include "thread_pool.hpp"

using std::string;

class ModManager {
  public:
	ModManager();
	~ModManager();

	void loadMods(string path);
	void addMod(string id, std::shared_ptr<Mod> mod);
	auto getMods() { return mods; }

  private:
	Settings *settings = Settings::getInstance();
	string path;
	std::unordered_map<string, std::shared_ptr<Mod>> mods;

	void loadModsParallel(std::filesystem::path path);
	void loadModsSequential(std::filesystem::path path);

	void processSteamWorkshopItem(string path);
	void processMod(string path, std::uint64_t steamId);
	void setupDependencies();
	bool isMapMod(string path);
	bool isTextureMod(string path);
	bool isNonEmptyExistsingDirectory(string path);

	Logger *logger;
};

#endif