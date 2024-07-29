#include "texture_pack_parser.hpp"
#include "binary_shift_reader.hpp"
#include "logger.hpp"
#include "texture.hpp"
#include "texture_pack.hpp"
#include "texture_page.hpp"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>

namespace filesystem = std::filesystem;

static auto logger = Logger::instance();
static std::string magicWord = "PZPK";
static std::vector<std::byte> deadBeef = {std::byte(0xEF), std::byte(0xBE),
										  std::byte(0xAD), std::byte(0xDE)};

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

std::shared_ptr<TexturePack> TexturePackParser::parseTexturePack() {

	std::shared_ptr<TexturePack> texturePack = std::make_shared<TexturePack>();

	texturePack->path = this->path;

	bool sanitizeRequired = this->needSanitization(magicWord);

	if (sanitizeRequired) {
		binaryShiftReader->readUint64(nullptr);
	}

	uint32_t texturePagesCount = 0;
	binaryShiftReader->readUint32(&texturePagesCount);

	logger->info("Parsing texture pack: " + this->path);
	logger->info("Texture pages count: " + std::to_string(texturePagesCount));

	for (std::size_t i = 0; i < texturePagesCount; ++i) {
		this->parsePage(texturePack);
	}

	if (sanitizeRequired) {
		binaryShiftReader->readUint32(nullptr);
	}

	std::shared_ptr<std::vector<std::byte>> imageBuffer =
		binaryShiftReader->getBuffer();

	size_t size = binaryShiftReader->getSize();
	size_t offset = binaryShiftReader->getOffset();

	binaryShiftReader->readBytes(imageBuffer->data() + offset, size - offset);

	imageBuffer->erase(imageBuffer->begin(), imageBuffer->begin() + offset);

	texturePack->imageBuffer = imageBuffer;

	return texturePack;
}

void TexturePackParser::parsePage(std::shared_ptr<TexturePack> texturePack) {

	std::shared_ptr<TexturePage> texturePage = std::make_shared<TexturePage>();

	uint32_t texturePageNameSize = 0;
	binaryShiftReader->readUint32(&texturePageNameSize);

	std::vector<std::byte> texturePageNameBytes(texturePageNameSize);

	binaryShiftReader->readChars((char *)texturePageNameBytes.data(),
								 texturePageNameSize);

	texturePage->setName(
		std::string((char *)texturePageNameBytes.data(), texturePageNameSize));

	uint32_t texturesCount = 0;
	binaryShiftReader->readUint32(&texturesCount);

	uint32_t texturePageAlpha = 0;
	binaryShiftReader->readUint32(&texturePageAlpha);

	texturePage->setAlpha(texturePageAlpha);

	logger->info("Texture page name: " + texturePage->getName());
	logger->info("Texture page textures: " + std::to_string(texturesCount));
	logger->info("Texture page alpha: " + std::to_string(texturePageAlpha));

	for (std::size_t i = 0; i < texturesCount; ++i) {
		this->parseTexture(texturePage);
	}

	bool sanitizeRequired = this->needSanitization(magicWord);

	uint32_t textureSize = 0;
	auto imageBuffer = vector<std::byte>();

	if (!sanitizeRequired) {
		auto position = binaryShiftReader->findBytesSequencePosition(deadBeef);

		textureSize = position - binaryShiftReader->getBuffer()->begin();
		textureSize -= binaryShiftReader->getOffset();

	} else {
		binaryShiftReader->readUint32(&textureSize);
	}

	imageBuffer.resize(textureSize);
	binaryShiftReader->readBytes(imageBuffer.data(), textureSize);

	if (!sanitizeRequired) {
		binaryShiftReader->readUint32(nullptr);
	}

	std::ofstream outputFile("image_buffer.png", std::ios::binary);
	outputFile.write(reinterpret_cast<const char *>(imageBuffer.data()),
					 imageBuffer.size());
	outputFile.close();

	texturePack->pages.push_back(texturePage);
}

void TexturePackParser::parseTexture(std::shared_ptr<TexturePage> texturePage) {

	std::shared_ptr<Texture> texture = std::make_shared<Texture>(	);

	uint32_t textureNameSize = 0;
	binaryShiftReader->readUint32(&textureNameSize);

	std::vector<std::byte> textureNameBytes(textureNameSize);

	binaryShiftReader->readChars((char *)textureNameBytes.data(),
								 textureNameSize);

	texture->name =
		std::string((char *)textureNameBytes.data(), textureNameSize);

	binaryShiftReader->readInt32(&texture->xCord);
	binaryShiftReader->readInt32(&texture->yCord);
	binaryShiftReader->readInt32(&texture->width);
	binaryShiftReader->readInt32(&texture->height);
	binaryShiftReader->readInt32(&texture->xCordOffset);
	binaryShiftReader->readInt32(&texture->yCordOffset);
	binaryShiftReader->readInt32(&texture->widthOffset);
	binaryShiftReader->readInt32(&texture->heightOffset);

	logger->info("Texture name: " + texture->name);

	texturePage->addTexture(texture);
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