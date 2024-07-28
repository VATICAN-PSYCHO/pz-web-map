#include "texture_pack_parser.hpp"
#include "binary_shift_reader.hpp"
#include "texture_pack.hpp"

#include "texture.hpp"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>

namespace filesystem = std::filesystem;

TexturePackParser::TexturePackParser(const std::string &path) {
	this->path = path;
	this->dataBuffer = std::make_shared<std::vector<std::byte>>(0);
}

TexturePackParser::~TexturePackParser() {}

std::shared_ptr<TexturePack> TexturePackParser::parse() {
	filesystem::path texturePackFile = filesystem::path(this->path);

	std::ifstream textureFile(texturePackFile, std::ios::in);

	if (!textureFile.is_open()) {
		printf("Failed to open texturepack file.\n");
		return nullptr;
	}

	printf("Texture pack file: %s\n", texturePackFile.c_str());
	printf("Texture pack size: %d MB\n",
		   (int

			)filesystem::file_size(texturePackFile) /
			   1024 / 1024);

	textureFile.seekg(0, std::ios::end);
	std::size_t fileSize = textureFile.tellg();
	textureFile.seekg(0, std::ios::beg);

	this->dataBuffer->resize(fileSize);

	textureFile.read(reinterpret_cast<char *>(this->dataBuffer->data()),
					 fileSize);
	textureFile.close();

	std::unique_ptr<BinaryShiftReader> reader =
		std::make_unique<BinaryShiftReader>(this->dataBuffer);

	std::shared_ptr<TexturePack> texturePack = std::make_shared<TexturePack>();

	texturePack->path = this->path;

	static std::string magicWord = "PZPK";

	bool sanitizeRequired = this->needSanitization(magicWord);

	if (sanitizeRequired) {
		reader->readUint64(nullptr);
	}

	reader->readUint32(nullptr);

	uint8_t texturePackNameSize = 0;
	reader->readUint32((uint32_t *)&texturePackNameSize);

	vector<byte> texturePackNameBytes(texturePackNameSize);

	reader->readChars((char *)texturePackNameBytes.data(), texturePackNameSize);

	texturePack->name =
		string((char *)texturePackNameBytes.data(), texturePackNameSize - 1);

	reader->readUint32((uint32_t *)&texturePack->size);
	reader->readUint32((uint32_t *)&texturePack->alpha);

	for (std::size_t i = 0; i < texturePack->size; ++i) {

		std::shared_ptr<Texture> texture = std::make_shared<Texture>();

		uint8_t textureNameSize = 0;
		reader->readUint32((uint32_t *)&textureNameSize);
		std::vector<byte> textureNameBytes(textureNameSize);

		reader->readChars((char *)textureNameBytes.data(), textureNameSize);

		texture->name =
			string((char *)textureNameBytes.data(), textureNameSize);

		reader->readInt32(&texture->xCord);
		reader->readInt32(&texture->yCord);
		reader->readInt32(&texture->width);
		reader->readInt32(&texture->height);
		reader->readInt32(&texture->xCordOffset);
		reader->readInt32(&texture->yCordOffset);
		reader->readInt32(&texture->widthOffset);
		reader->readInt32(&texture->heightOffset);

		texturePack->textures.push_back(texture);
	}

	if (sanitizeRequired) {
		reader->readUint32(nullptr);
	}

	std::shared_ptr<std::vector<std::byte>> imageBuffer =
		std::make_shared<std::vector<std::byte>>(fileSize -
												 reader->getOffset());

	reader->readChars((char *)imageBuffer->data(),
					  fileSize - reader->getOffset());

	texturePack->imageBuffer = imageBuffer;

	return texturePack;
}

bool TexturePackParser::needSanitization(std::string sanitizeText) {

	if (this->dataBuffer->size() < sanitizeText.size()) {
		return false;
	}

	for (std::size_t i = 0; i < sanitizeText.size(); ++i) {
		if (this->dataBuffer->at(i) != std::byte(sanitizeText.at(i))) {
			return false;
		}
	}

	return true;
}