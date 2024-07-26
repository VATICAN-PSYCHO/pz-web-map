#include "include/binary_shift_reader.hpp"
#include "include/custom_structs.hpp"

#include <cstdint>
#include <deque>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "yaml-cpp/yaml.h"

using std::byte;
using std::deque;
using std::string;
using std::vector;
namespace filesystem = std::filesystem;

bool needSanitization(deque<byte> &data, string &sanitizeText) {

	if (data.size() < sanitizeText.size()) {
		return false;
	}

	for (std::size_t i = 0; i < sanitizeText.size(); ++i) {
		if (data[i] != byte(sanitizeText[i])) {
			return false;
		}
	}

	return true;
}

int main(int argc, char *argv[]) {

	YAML::Node config = YAML::LoadFile("config.yaml");

	if (!config["game_dir"]) {
		printf("Game directory not found in config file.\n");
		return 1;
	}

	if (!config["mods_dir"]) {
		printf("Game mods directory not found in config file.\n");
		return 1;
	}

	string _gameDir = config["game_dir"].as<string>();
	string _gameModsDir = config["mods_dir"].as<string>();

	filesystem::path gameDir = filesystem::path(_gameDir);
	filesystem::path gameModsDir = filesystem::path(_gameModsDir);

	if (!filesystem::exists(gameDir)) {
		printf("Game directory does not exist.\n");
		return 1;
	}

	if (!filesystem::exists(gameDir)) {
		printf("Game directory does not exist.\n");
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

	filesystem::path texturePacksDir = gameDir / "media" / "texturepacks";

	if (!filesystem::exists(texturePacksDir)) {
		printf("Texturepacks directory not found.\n");
		return 1;
	}

	filesystem::path texturesDir =
		filesystem::current_path() / "var" / "textures";

	filesystem::create_directories(texturesDir);

	vector<string> includePacks = config["include_packs"].as<vector<string>>();

	for (const auto &entry : filesystem::directory_iterator(texturePacksDir)) {

		if (entry.is_directory()) {
			continue;
		}

		if (includePacks.size() > 0) {
			bool found = false;

			for (size_t i = 0; i < includePacks.size(); ++i) {
				if (entry.path().filename() == includePacks[i]) {
					found = true;
					break;
				}
			}

			if (!found) {
				continue;
			}
		}

		string filePath = entry.path();
		string fileName = entry.path().filename();

		std::ifstream textureFile(entry.path(), std::ios::in);

		if (!textureFile.is_open()) {
			printf("Failed to open file.\n");
			return 1;
		}

		printf("Texture Pack File: %s\n", fileName.c_str());

		textureFile.seekg(0, std::ios::end);
		std::size_t fileSize = textureFile.tellg();
		textureFile.seekg(0, std::ios::beg);

		vector<byte> tmpBuffer(fileSize);

		textureFile.read((char *)tmpBuffer.data(), fileSize);
		textureFile.close();

		deque<byte> fileData(tmpBuffer.begin(), tmpBuffer.end());

		string magicWord = "PZPK";

		bool toSanitize = needSanitization(fileData, magicWord);

		BinaryShiftReader *reader = nullptr;
		reader = new BinaryShiftReader(&fileData);

		if (toSanitize) {
			reader->read_uint64(nullptr);
		}

		TexturePack texturePack;

		reader->read_uint32(nullptr);

		uint8_t texturePackNameSize = 0;
		reader->read_int32((int32_t *)&texturePackNameSize);

		vector<byte> texturePackNameBytes;

		reader->read_bytes_vector(&texturePackNameBytes, texturePackNameSize);

		texturePack.name =
			string((char *)texturePackNameBytes.data(), texturePackNameSize);

		reader->read_int32((int32_t *)&texturePack.size);
		reader->read_int32((int32_t *)&texturePack.alpha);

		for (size_t i = 0; i < texturePack.size; ++i) {

			Texture texture;

			uint8_t textureNameSize = 0;
			reader->read_int32((int32_t *)&textureNameSize);

			vector<byte> textureNameBytes;

			reader->read_bytes_vector(&textureNameBytes, textureNameSize);

			texture.name =
				string((char *)textureNameBytes.data(), textureNameSize);

			reader->read_int32((int32_t *)&texture.x);
			reader->read_int32((int32_t *)&texture.y);
			reader->read_int32((int32_t *)&texture.w);
			reader->read_int32((int32_t *)&texture.h);
			reader->read_int32((int32_t *)&texture.ox);
			reader->read_int32((int32_t *)&texture.oy);
			reader->read_int32((int32_t *)&texture.ow);
			reader->read_int32((int32_t *)&texture.oh);
		}

		printf("Texture Pack: %s\n", texturePack.name.c_str());

		auto textureChunk = fileData;

		filesystem::path texturePackDir = texturesDir / texturePack.name;

		filesystem::create_directory(texturePackDir);

		filesystem::path texturePackFile =
			texturesDir / (texturePack.name + ".png");

		std::ofstream texturePackFileOut(texturePackFile, std::ios::out);

		if (!texturePackFileOut.is_open()) {
			printf("Failed to open file.\n");
			return 1;
		}

		for (const auto &byte : textureChunk) {
			texturePackFileOut.write(reinterpret_cast<const char *>(&byte),
									 sizeof(byte));
		}

		texturePackFileOut.close();

		delete reader;
	}

	return 0;
}