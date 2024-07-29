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
	filesystem::path texturePackFile = filesystem::path(this->path);
	std::ifstream textureFile(texturePackFile, std::ios::in);

	if (!textureFile.is_open()) {
		throw std::runtime_error("Failed to open file: " + this->path);
	}

	textureFile.seekg(0, std::ios::end);
	auto fileSize = textureFile.tellg();
	textureFile.seekg(0, std::ios::beg);

	this->binaryShiftReader = std::make_shared<BinaryShiftReader>(
		std::make_shared<std::vector<std::byte>>(fileSize));

	auto buffer = this->binaryShiftReader->getBuffer();

	textureFile.read((char *)buffer->data(), fileSize);
	textureFile.close();
}

TexturePackParser::~TexturePackParser() {}

std::shared_ptr<TexturePack> TexturePackParser::parse() {

	std::shared_ptr<TexturePack> texturePack = std::make_shared<TexturePack>();

	texturePack->path = this->path;

	static std::string magicWord = "PZPK";

	bool sanitizeRequired = this->needSanitization(magicWord);

	if (sanitizeRequired) {
		binaryShiftReader->readUint64(nullptr);
	}

	binaryShiftReader->readUint32(nullptr);

	uint8_t texturePackNameSize = 0;
	binaryShiftReader->readUint32((uint32_t *)&texturePackNameSize);

	vector<byte> texturePackNameBytes(texturePackNameSize);

	binaryShiftReader->readChars((char *)texturePackNameBytes.data(),
								 texturePackNameSize);

	texturePack->name =
		string((char *)texturePackNameBytes.data(), texturePackNameSize - 1);

	binaryShiftReader->readUint32((uint32_t *)&texturePack->size);
	binaryShiftReader->readUint32((uint32_t *)&texturePack->alpha);

	for (std::size_t i = 0; i < texturePack->size; ++i) {

		std::shared_ptr<Texture> texture = std::make_shared<Texture>();

		uint8_t textureNameSize = 0;
		binaryShiftReader->readUint32((uint32_t *)&textureNameSize);
		std::vector<byte> textureNameBytes(textureNameSize);

		binaryShiftReader->readChars((char *)textureNameBytes.data(),
									 textureNameSize);

		texture->name =
			string((char *)textureNameBytes.data(), textureNameSize);

		binaryShiftReader->readInt32(&texture->xCord);
		binaryShiftReader->readInt32(&texture->yCord);
		binaryShiftReader->readInt32(&texture->width);
		binaryShiftReader->readInt32(&texture->height);
		binaryShiftReader->readInt32(&texture->xCordOffset);
		binaryShiftReader->readInt32(&texture->yCordOffset);
		binaryShiftReader->readInt32(&texture->widthOffset);
		binaryShiftReader->readInt32(&texture->heightOffset);

		texturePack->textures.push_back(texture);
	}

	if (sanitizeRequired) {
		binaryShiftReader->readUint32(nullptr);
	}

	std::shared_ptr<std::vector<std::byte>> imageBuffer =
		binaryShiftReader->getBuffer();

	size_t size = binaryShiftReader->getSize();
	size_t offset = binaryShiftReader->getOffset();

	binaryShiftReader->readBytes(imageBuffer->data() + offset, size - offset);

	return texturePack;
}

bool TexturePackParser::needSanitization(std::string sanitizeText) {

	if (this->binaryShiftReader->getSize() < sanitizeText.size()) {
		return false;
	}

	for (std::size_t i = 0; i < sanitizeText.size(); ++i) {
		if (this->binaryShiftReader->at(i) != std::byte(sanitizeText.at(i))) {
			return false;
		}
	}

	return true;
}