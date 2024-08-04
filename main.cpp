#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <regex>
#include <string>
#include <thread>
#include <vector>

// #include <CL/cl.h>
#include <opencv2/opencv.hpp>

#include "config.hpp"
#include "file_system.hpp"
#include "logger.hpp"
#include "mod_manager.hpp"
#include "texture.hpp"
#include "texture_library.hpp"
#include "texture_pack_reader.hpp"
#include "texture_page.hpp"
#include "thread_pool.hpp"

namespace filesystem = std::filesystem;

void validate();
void extract();

int main() {

	auto logger = Logger::getInstance();
	auto config = Config::getInstance();

	config->load();

	auto gameDir = config->directories.gameDir;
	auto steamWorkshopDir = config->directories.modsDir;
	auto outputDir = config->directories.outputDir;
	auto includePacks = config->includePacksNames;

	validate();

	// std::shared_ptr<ThreadPool> threadPool =
	// 	std::make_shared<ThreadPool>(config->execution.threads);

	TextureLibrary textureLibrary(config);
	ModManager modManager(steamWorkshopDir, &textureLibrary);

	auto modsEnabled = modManager.loadMods();

	textureLibrary.proccessTexturePacks();

	filesystem::create_directories(outputDir);

	auto mapDirectory =
		filesystem::path(gameDir) / "media" / "maps" / "Muldraugh, KY";

	auto mapDirectoryExists = FileSystem::validatePath(mapDirectory);

	if (!mapDirectoryExists) {
		logger->error("Map directory does not exist.");
		return 1;
	}

	auto lotHeaderRegex = std::regex("^(\\d+)_(\\d+).lotheader$");

	for (const auto &mapDirEntry :
		 filesystem::directory_iterator(mapDirectory)) {

		if (mapDirEntry.is_directory()) {
			continue;
		}

		const auto &filePath = mapDirEntry.path();
		const auto &fileName = filePath.filename().string();

		std::smatch match;
		if (std::regex_match(fileName, match, lotHeaderRegex)) {
			const auto &xCoordStr = match[1].str();
			const auto &yCoordStr = match[2].str();

			int xCoord = std::stoi(xCoordStr);
			int yCoord = std::stoi(yCoordStr);

			// Process the lot header file
			// ...

			logger->info("Lot header file: " + fileName);
			logger->info("X Coord: " + std::to_string(xCoord) +
						 ", Y Coord: " + std::to_string(yCoord));
		}
	}

	return 0;
}

void validate() {
	auto logger = Logger::getInstance();
	auto config = Config::getInstance();

	auto gameDireExists = FileSystem::validatePath(config->directories.gameDir);

	if (!gameDireExists) {
		logger->error("Game directory does not exist.");
		exit(1);
	}

	auto modsDirExists = FileSystem::validatePath(config->directories.modsDir);

	if (!modsDirExists) {
		logger->error("Mods directory does not exist.");
		exit(1);
	}

	filesystem::path mapsDirectory =
		filesystem::path(config->directories.gameDir) / "media" / "maps";

	auto mapsDirectoryExists = FileSystem::validatePath(mapsDirectory);

	if (!mapsDirectoryExists) {
		logger->error("Maps directory does not exist.");
		exit(1);
	}

	filesystem::path texturePacksDirectory =
		filesystem::path(config->directories.gameDir) / "media" /
		"texturepacks";

	auto texturePacksDirectoryExists =
		FileSystem::validatePath(texturePacksDirectory);

	if (!texturePacksDirectoryExists) {
		logger->error("Texture packs directory does not exist.");
		exit(1);
	}
}
