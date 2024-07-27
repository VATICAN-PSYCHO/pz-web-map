#include "include/binary_shift_reader.hpp"
#include "include/mod_manager.hpp"
#include "include/settings.hpp"
#include "include/texture.hpp"
#include "include/texture_pack.hpp"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "yaml-cpp/yaml.h"

using std::byte;
using std::string;
using std::vector;

namespace filesystem = std::filesystem;

ModManager modManager;

bool sanitizeRequired = false;
string magicWord = "PZPK";

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

void validate_path(string unsafePath) {
	if (!filesystem::exists(unsafePath)) {
		printf("Path does not exist: %s\n", unsafePath.c_str());
		exit(1);
	}

	if (!filesystem::is_directory(unsafePath)) {
		printf("Path is not a directory: %s\n", unsafePath.c_str());
		exit(1);
	}
}

int main(int argc, char *argv[]) {

	Settings *settings = Settings::getInstance();

	settings->loadSettings();

	string gameDir = settings->getGameDir();
	string steamWorkshopDir = settings->steamWorkshopDir();
	vector<string> includePacks = settings->getIncludePacksDir();

	validate_path(gameDir);
	validate_path(steamWorkshopDir);

	modManager.loadMods(steamWorkshopDir);

	auto mods = modManager.getMods();

	if (includePacks.size() == 0) {
		printf("No texture packs to include.\n");
		return 1;
	}

	bool mediaDirFound = false;

	for (const auto &entry : filesystem::directory_iterator(gameDir)) {

		if (entry.is_directory()) {
			if (entry.path().filename() == "media") {
				mediaDirFound = true;
				break;
			}
		}
	}

	if (!mediaDirFound) {
		printf("Media directory not found.\n");
		return 1;
	}

	vector<filesystem::path> texturePacks;

	filesystem::path texturePacksDir =
		filesystem::path(gameDir) / "media" / "texturepacks";

	if (!filesystem::exists(texturePacksDir)) {
		printf("Game texturepacks directory not found.\n");
		return 1;
	}

	filesystem::path texturesDir =
		filesystem::current_path() / "var" / "textures";

	filesystem::create_directories(texturesDir);

	for (const auto &texturePackDirEntry :
		 filesystem::directory_iterator(texturePacksDir)) {

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

		string texturePackFilePath = texturePackDirEntry.path();

		string texturePackFileName = texturePackDirEntry.path().filename();
		string texturePackBaseName = texturePackDirEntry.path().stem();

		std::ifstream textureFile(texturePackFilePath, std::ios::in);

		if (!textureFile.is_open()) {
			printf("Failed to open texturepack file.\n");
			continue;
		}

		std::unique_ptr<TexturePack> texturePack =
			std::make_unique<TexturePack>();

		printf("Texture pack file: %s\n", texturePackFileName.c_str());
		printf("Texture pack size: %d MB\n",
			   filesystem::file_size(texturePackFilePath) / 1024 / 1024);

		textureFile.seekg(0, std::ios::end);
		std::size_t fileSize = textureFile.tellg();
		textureFile.seekg(0, std::ios::beg);

		std::size_t dataBufferOffset = 0;
		vector<byte> *dataBuffer = new vector<byte>(fileSize);

		textureFile.read((char *)dataBuffer->data(), fileSize);
		textureFile.close();

		std::unique_ptr<BinaryShiftReader> reader =
			std::make_unique<BinaryShiftReader>(dataBuffer, &dataBufferOffset);

		sanitizeRequired = needSanitization(dataBuffer, &magicWord);

		if (sanitizeRequired) {
			reader->read_uint64(nullptr);
		}

		reader->read_uint32(nullptr);

		uint8_t texturePackNameSize = 0;
		reader->read_uint32((uint32_t *)&texturePackNameSize);

		vector<byte> texturePackNameBytes(texturePackNameSize);

		reader->read_chars((char *)texturePackNameBytes.data(),
						   texturePackNameSize);

		reader->read_uint32((uint32_t *)&texturePack->size);
		reader->read_uint32((uint32_t *)&texturePack->alpha);

		for (std::size_t i = 0; i < texturePack->size; ++i) {

			std::unique_ptr<Texture> texture = std::make_unique<Texture>();

			uint8_t textureNameSize = 0;
			reader->read_uint32((uint32_t *)&textureNameSize);
			vector<byte> textureNameBytes(textureNameSize);
			reader->read_chars((char *)textureNameBytes.data(),
							   textureNameSize);

			texture->name =
				string((char *)textureNameBytes.data(), textureNameSize);

			reader->read_uint32((uint32_t *)&texture->x);
			reader->read_uint32((uint32_t *)&texture->y);
			reader->read_uint32((uint32_t *)&texture->w);
			reader->read_uint32((uint32_t *)&texture->h);
			reader->read_uint32((uint32_t *)&texture->ox);
			reader->read_uint32((uint32_t *)&texture->oy);
			reader->read_uint32((uint32_t *)&texture->ow);
			reader->read_uint32((uint32_t *)&texture->oh);
		}

		if (sanitizeRequired) {
			uint32_t textureChunkSize = 0;
			reader->read_uint32(&textureChunkSize);
		}

		printf("Texture Pack: %s\n", texturePack->name.c_str());

		filesystem::path texturePackDir = texturesDir / texturePack->name;

		filesystem::create_directory(texturePackDir);

		filesystem::path textureChunkFile =
			texturesDir / texturePack->name / (texturePackBaseName + ".png");

		std::fstream texturePackFileOutStream;

		texturePackFileOutStream.open(textureChunkFile,
									  std::ios::out | std::ios::binary);

		if (!texturePackFileOutStream.is_open()) {
			printf("Failed to open texture file.\n");
			continue;
		}

		texturePackFileOutStream.write((char *)dataBuffer->data() +
										   dataBufferOffset,
									   dataBuffer->size() - dataBufferOffset);

		texturePackFileOutStream.close();
	}

	return 0;
}