#include "include/binary_shift_reader.hpp"
#include "include/custom_structs.hpp"
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

bool needSanitization(vector<byte> &data, string &sanitizeText) {

	if (data.size() < sanitizeText.size()) {
		return false;
	}

	for (size_t i = 0; i < sanitizeText.size(); ++i) {
		if (data[i] != byte(sanitizeText[i])) {
			return false;
		}
	}

	return true;
}

int main(int argc, char *argv[]) {

	if (argc < 3) {
		printf("Please provide the game directory and gameMods "
			   "directory as "
			   "command line arguments.\n");
		return 1;
	}

	auto gameDirArg = argv[1];
	auto gameModsDirArg = argv[2];

	printf("Game Directory: %s\n", gameDirArg);
	printf("Game Mods Directory: %s\n", gameModsDirArg);

	auto gameDir = filesystem::path(gameDirArg);

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

	filesystem::path texturesDir = filesystem::current_path() / "textures";

	filesystem::create_directory(texturesDir);

	filesystem::path texturePacksDir = gameDir / "media" / "texturepacks";

	if (!filesystem::exists(texturePacksDir)) {
		printf("Texturepacks directory not found.\n");
		return 1;
	}

	for (const auto &entry : filesystem::directory_iterator(texturePacksDir)) {

		if (!entry.is_directory()) {

			string filePath = entry.path();
			string fileName = entry.path().filename();

			if (filePath.find(".pack") != string::npos) {

				ifstream textureFile(entry.path(), ios::in);

				if (!textureFile.is_open()) {
					printf("Failed to open file.\n");
					return 1;
				}

				textureFile.seekg(0, ios::end);
				size_t fileSize = textureFile.tellg();
				textureFile.seekg(0, ios::beg);

				string magicWord = "PZPK";
				vector<byte> fileData(fileSize);

				bool toSanitize = needSanitization(fileData, magicWord);

				textureFile.read((char *)fileData.data(), fileSize);
				textureFile.close();

				BinaryShiftReader *reader = nullptr;
				reader = new BinaryShiftReader(&fileData);

				if (toSanitize) {
					reader->read_bytes(&fileData, 8);
				}

				TexturePack texturePack;

				reader->read_uint32(nullptr);

				uint8_t texturePackNameSize = 0;
				reader->read_int32((int32_t *)&texturePackNameSize);

				vector<byte> texturePackNameBytes;

				reader->read_bytes(&texturePackNameBytes, texturePackNameSize);

				texturePack.name = string((char *)texturePackNameBytes.data(),
										  texturePackNameSize);

				reader->read_int32((int32_t *)&texturePack.size);
				reader->read_int32((int32_t *)&texturePack.alpha);

				for (size_t i = 0; i < texturePack.size; ++i) {

					Texture texture;

					uint8_t textureNameSize = 0;
					reader->read_int32((int32_t *)&textureNameSize);

					vector<byte> textureNameBytes;

					reader->read_bytes(&textureNameBytes, textureNameSize);

					texture.name = string((char *)textureNameBytes.data(),
										  textureNameSize);

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

				filesystem::path texturePackDir =
					texturesDir / texturePack.name;

				filesystem::create_directory(texturePackDir);

				filesystem::path texturePackFile =
					texturesDir / (texturePack.name + ".png");

				ofstream texturePackFileOut(texturePackFile, ios::out);

				if (!texturePackFileOut.is_open()) {
					printf("Failed to open file.\n");
					return 1;
				}

				texturePackFileOut.write((char *)textureChunk.data(),
										 textureChunk.size());

				delete reader;
			}
		}
	}

	return 0;
}