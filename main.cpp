#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

// #include <CL/cl.h>
// #include <opencv2/core.hpp>
// #include <opencv2/highgui.hpp>
// #include <opencv2/imgcodecs.hpp>
// #include <opencv2/imgproc.hpp>
// #include <opencv2/opencv.hpp>

#include "file_system.hpp"
#include "logger.hpp"
#include "mod_manager.hpp"
#include "settings.hpp"
#include "texture_pack_parser.hpp"

using std::byte;

using std::string;
using std::vector;

namespace filesystem = std::filesystem;

// using namespace cv;

static Settings settings;
static ModManager modManager;

auto logger = Logger::instance();

bool needSanitization(vector<byte> *data, string *sanitizeText) {

	if (data->size() < sanitizeText->size()) {
		return false;
	}

	for (std::size_t i = 0; i < sanitizeText->size(); ++i) {
		if (data->at(i) != byte(sanitizeText->at(i))) {
			return false;
		}
	}

	return true;
}

int main() {

	settings.loadSettings();

	string gameDir = settings.getGameDir();
	string steamWorkshopDir = settings.steamWorkshopDir();
	vector<string> includePacks = settings.getIncludePacksDir();

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

	if (includePacks.size() == 0) {
		printf("No texture packs to include.\n");
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

	std::vector<std::shared_ptr<TexturePack>> texturePacks;

	for (const auto &texturePackPath : *texturePacksPaths) {

		std::shared_ptr<TexturePackParser> texturePackParser =
			std::make_shared<TexturePackParser>(texturePackPath.string());

		std::shared_ptr<TexturePack> texturePack = texturePackParser->parse();

		if (texturePack == nullptr) {
			printf("Failed to parse texture pack: %s\n",
				   texturePackPath.c_str());
			continue;
		}

		texturePacks.push_back(texturePack);
	}

	for (const auto &texturePack : texturePacks) {
		printf("Texture path: %s\n", texturePack->getPath().c_str());
		printf("Texture pack: %s\n", texturePack->getName().c_str());

		// auto imageBuffer = texturePack->getBuffer();

		// std::shared_ptr<vector<uchar>> ucharBuffer =
		// 	std::make_shared<vector<uchar>>();

		// std::transform(imageBuffer->begin(), imageBuffer->end(),
		// 			   std::back_inserter(*ucharBuffer),
		// 			   [](byte b) { return static_cast<uchar>(b); });

		// std::shared_ptr<Mat> image =
		// 	std::make_shared<Mat>(imdecode(*ucharBuffer, IMREAD_UNCHANGED));

		// printf("Width: %d, Height: %d\n", image->cols, image->rows);

		// std::ofstream outputFile;
		// outputFile.open("test.png", std::ios::binary);
		// if (outputFile.is_open()) {
		// 	outputFile.write(
		// 		reinterpret_cast<const char *>(imageBuffer->data()),
		// 		imageBuffer->size());
		// 	outputFile.close();
		// }

		// for (const auto &texture : texturePack->getTextures()) {
		// 	auto name = texture->getName();

		// 	auto x = texture->getXCord();
		// 	auto y = texture->getYCord();
		// 	auto width = texture->getWidth();
		// 	auto height = texture->getHeight();

		// 	auto ox = texture->getXCordOffset();
		// 	auto oy = texture->getYOffset();
		// 	auto ow = texture->getWidthOffset();
		// 	auto oh = texture->getHeightOffset();
		// 	;

		// 	ox -= ow >> 1;
		// 	oy -= oh;

		// 	printf("Texture: %s\n", name.c_str());

		// 	printf("X: %d, Y: %d, Width: %d, Height: %d\n", x, y, width,
		// 		   height);

		// 	std::shared_ptr<Mat> textureImage =
		// 		std::make_shared<Mat>(*image, Rect(x, y, width, height));

		// 	filesystem::path outputDir =
		// 		filesystem::path(settings.getOutputDir());

		// 	if (outputDir.is_relative()) {
		// 		outputDir = filesystem::current_path() / outputDir;
		// 	}

		// 	filesystem::path texturePath = outputDir / texturePack->getName();

		// 	filesystem::create_directories(texturePath);

		// 	// imwrite(texturePath / (texture->getName() + ".png"),
		// 	// *textureImage);
		// }
	}

	return 0;
}
