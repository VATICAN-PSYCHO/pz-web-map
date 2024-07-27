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

bool needSanitization(deque<byte> *data, string *sanitizeText) {

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

bool sanitizeRequired = false;

void de_allocate(void *ptr) {
	if (ptr != nullptr) {
		delete ptr;
		ptr = nullptr;
	}
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

	string magicWord = "PZPK";
	// bool sanitizeRequired = false;

	for (const auto &texturePackDirEntry :
		 filesystem::directory_iterator(texturePacksDir)) {

		if (texturePackDirEntry.is_directory()) {
			continue;
		}

		if (includePacks.size() > 0) {
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
		}

		string texturePackFilePath = texturePackDirEntry.path();
		string texturePackFileName = texturePackDirEntry.path().filename();
		string texturePackBaseName = texturePackDirEntry.path().stem();

		std::ifstream textureFile(texturePackFilePath, std::ios::in);

		if (!textureFile.is_open()) {
			printf("Failed to open texturepack file.\n");
			continue;
		}

		printf("Texture pack file: %s\n", texturePackFileName.c_str());
		printf("Texture pack size: %d MB\n",
			   filesystem::file_size(texturePackFilePath) / 1024 / 1024);

		textureFile.seekg(0, std::ios::end);
		std::size_t fileSize = textureFile.tellg();
		textureFile.seekg(0, std::ios::beg);

		deque<byte> *fileData = new deque<byte>(fileSize);

		vector<byte> *tmpBuffer = new vector<byte>(fileSize);

		textureFile.read((char *)tmpBuffer->data(), fileSize);

		fileData->assign(tmpBuffer->begin(), tmpBuffer->end());

		tmpBuffer->clear();
		de_allocate(tmpBuffer);

		textureFile.close();

		BinaryShiftReader *reader = new BinaryShiftReader(fileData);

		sanitizeRequired = needSanitization(fileData, &magicWord);

		if (sanitizeRequired) {
			reader->read_uint64(nullptr);
		}

		TexturePack *texturePack = new TexturePack();

		reader->read_uint32(nullptr);

		uint8_t texturePackNameSize = 0;
		reader->read_uint32((uint32_t *)&texturePackNameSize);

		vector<byte> texturePackNameBytes(texturePackNameSize);

		reader->read_bytes_vector(&texturePackNameBytes, texturePackNameSize);

		texturePack->name = string((char *)texturePackNameBytes.data(),
								   texturePackNameSize - 1);

		reader->read_uint32((uint32_t *)&texturePack->size);
		reader->read_uint32((uint32_t *)&texturePack->alpha);

		for (std::size_t i = 0; i < texturePack->size; ++i) {

			Texture *texture = new Texture();

			uint8_t textureNameSize = 0;
			reader->read_uint32((uint32_t *)&textureNameSize);
			vector<byte> textureNameBytes(textureNameSize);
			reader->read_bytes_vector(&textureNameBytes, textureNameSize);

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

			de_allocate(texture);
		}

		if (sanitizeRequired) {
			uint32_t textureChunkSize = 0;
			reader->read_uint32(&textureChunkSize);
		}

		printf("Texture Pack: %s\n", texturePack->name.c_str());

		auto textureChunk = fileData;

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

		for (std::size_t i = 0; i < textureChunk->size(); ++i) {
			texturePackFileOutStream << (char)textureChunk->at(i);
		}

		texturePackFileOutStream.close();

		de_allocate(texturePack);
		de_allocate(fileData);
		de_allocate(reader);
	}

	return 0;
}