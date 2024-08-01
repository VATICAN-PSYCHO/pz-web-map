#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

// #include <CL/cl.h>
#include <opencv2/opencv.hpp>

#include "file_system.hpp"
#include "logger.hpp"
#include "mod_manager.hpp"
#include "settings.hpp"
#include "texture_pack_parser.hpp"
#include "texture_page.hpp"
#include "thread_pool.hpp"

using std::byte;

using std::string;
using std::vector;

namespace filesystem = std::filesystem;

int main() {

	auto logger = Logger::getInstance();
	auto settings = Settings::getInstance();

	settings->loadSettings();

	string gameDir = settings->gameDir;
	string steamWorkshopDir = settings->workshopDir;
	vector<string> includePacks = settings->includePacksDir;

	std::shared_ptr<ThreadPool> threadPool =
		std::make_shared<ThreadPool>(settings->execution.threads);

	ModManager modManager(threadPool);

	bool gameDirExists = FileSystem::validatePath(gameDir);

	if (!gameDirExists) {
		logger->error("Game directory does not exist.");
		return 1;
	}

	bool steamWorkshopDirExists = FileSystem::validatePath(steamWorkshopDir);

	if (!steamWorkshopDirExists) {
		logger->error("Steam workshop directory does not exist.");
		return 1;
	}

	modManager.loadMods(steamWorkshopDir);

	auto mods = modManager.getMods();

	logger->info("Total mods count: " + std::to_string(mods.size()));

	for (const auto &mod : mods) {
		logger->info("Mod name: " + mod.second.get()->getName());
	}

	if (includePacks.size() == 0) {
		logger->error("No texture packs to include.");
		return 1;
	}

	filesystem::path texturePacksDirectory =
		filesystem::path(gameDir) / "media" / "texturepacks";

	filesystem::path mapsDirectory =
		filesystem::path(gameDir) / "media" / "maps";

	auto mapsDirectoryExists = FileSystem::validatePath(mapsDirectory);

	if (!mapsDirectoryExists) {
		logger->error("Maps directory does not exist.");
		return 1;
	}

	auto texturePacksDirectoryExists =
		FileSystem::validatePath(texturePacksDirectory);

	if (!texturePacksDirectoryExists) {
		logger->error("Texture packs directory does not exist.");
		return 1;
	}

	std::shared_ptr<vector<filesystem::path>> texturePacksPaths =
		std::make_shared<vector<filesystem::path>>();

	for (const auto &texturePackDirEntry :
		 filesystem::directory_iterator(texturePacksDirectory)) {

		if (texturePackDirEntry.is_directory()) {
			continue;
		}

		bool found = false;

		for (std::size_t i = 0; i < includePacks.size(); ++i) {
			if (texturePackDirEntry.path().filename() == includePacks[i]) {
				found = true;
				break;
			}
		}

		if (!found) {
			continue;
		}

		texturePacksPaths->push_back(texturePackDirEntry.path());
	}

	for (const auto &mod : mods) {
		auto modPath = mod.second.get()->getPath();
		auto modType = mod.second.get()->getType();

		if (modType == ModType::MAP) {
			continue;
		}

		filesystem::path modTexturePackDirectory =
			filesystem::path(modPath) / "media" / "texturepacks";

		for (const auto &texturePackDirEntry :
			 filesystem::directory_iterator(modTexturePackDirectory)) {

			if (texturePackDirEntry.is_directory()) {
				continue;
			}

			if (texturePackDirEntry.path().extension() != ".pack") {
				continue;
			}

			texturePacksPaths->push_back(texturePackDirEntry.path());
		}
	}

	std::unique_ptr<vector<std::shared_ptr<TexturePack>>> texturePacks =
		std::make_unique<vector<std::shared_ptr<TexturePack>>>();

	std::vector<std::shared_ptr<TexturePackParser>> texturePackParsers;

	for (const auto &texturePackPath : *texturePacksPaths) {

		std::shared_ptr<TexturePackParser> texturePackParser =
			std::make_shared<TexturePackParser>(texturePackPath.string());

		texturePackParsers.push_back(texturePackParser);

		auto texturePack = texturePackParser->parseTexturePack();

		texturePacks->push_back(texturePack);
	}

	filesystem::path outputDir = settings->outputDir;

	filesystem::create_directories(outputDir);

	for (const auto &texturePack : *texturePacks) {
		logger->info("Texture pack: " + texturePack->getPath());

		auto pages = texturePack->getPages();

		for (const auto &page : pages) {
			logger->info("Page: " + page->getName());
			auto pageTextures = page->getTextures();

			std::shared_ptr<vector<uchar>> ucharBuffer =
				std::make_shared<vector<uchar>>();

			std::transform(page->start, page->end,
						   std::back_inserter(*ucharBuffer),
						   [](byte b) { return static_cast<uchar>(b); });

			std::shared_ptr<cv::Mat> image = std::make_shared<cv::Mat>(
				cv::imdecode(*ucharBuffer, cv::IMREAD_UNCHANGED));

			filesystem::path pageDirectory = outputDir / page->getName();
			filesystem::create_directories(pageDirectory);

			for (const auto &texture : pageTextures) {
				threadPool->enqueue([texture, image, pageDirectory, logger]() {
					auto name = texture->getName();

					auto x = texture->getXCord();
					auto y = texture->getYCord();
					auto width = texture->getWidth();
					auto height = texture->getHeight();

					// auto ox = texture->getXCordOffset();
					// auto oy = texture->getYCordOffset();
					// auto ow = texture->getWidthOffset();
					// auto oh = texture->getHeightOffset();

					logger->info("Texture: " + name);
					logger->info("X: " + std::to_string(x) +
								 ", Y: " + std::to_string(y) +
								 ", Width: " + std::to_string(width) +
								 ", Height: " + std::to_string(height));

					auto textureImage = texture->render(image);

					filesystem::path texturePath = pageDirectory / name;

					try {
						cv::imwrite(texturePath.string() + ".png",
									*textureImage);
					} catch (const cv::Exception &e) {
						logger->error(e.what());
					}
				});
			}

			threadPool->wait();
		}
	}

	return 0;
}
