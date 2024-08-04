#include "texture_pack_reader.hpp"
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

static auto logger = Logger::getInstance();
static std::string magicWord = "PZPK";
static std::vector<std::byte> deadBeef = {std::byte(0xEF), std::byte(0xBE),
										  std::byte(0xAD), std::byte(0xDE)};

TexturePackReader::TexturePackReader(const std::string &path) {
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

	this->version = TexturePackVersion::Unknown;

	this->determineVersion();
}

TexturePackReader::~TexturePackReader() {}

void TexturePackReader::determineVersion() {

	static std::string optionalFileHeader = "PZPK";

	for (std::size_t i = 0; i < optionalFileHeader.size(); ++i) {
		if (this->binaryShiftReader->at(i) !=
			std::byte(optionalFileHeader.at(i))) {
			version = TexturePackVersion::V1;
			return;
		}
	}

	version = TexturePackVersion::V2;
}

std::shared_ptr<TexturePack> TexturePackReader::read() {

	std::shared_ptr<TexturePack> texturePack = std::make_shared<TexturePack>();

	if (this->version == TexturePackVersion::V2) {
		binaryShiftReader->readInt64(nullptr);
	}

	uint32_t texturePagesCount = 0;
	binaryShiftReader->readUint32(&texturePagesCount);

	logger->info("Parsing texture pack: " + this->path);
	logger->info("Texture pack size: " +
				 std::to_string(binaryShiftReader->getSize() / (1024 * 1024)) +
				 " MB");
	logger->info("Texture pages count: " + std::to_string(texturePagesCount));

	for (std::size_t i = 0; i < texturePagesCount; ++i) {
		this->readPage(texturePack);
	}

	if (this->version == TexturePackVersion::V2) {
		binaryShiftReader->readInt32(nullptr);
	}

	return texturePack;
}

void TexturePackReader::readPage(std::shared_ptr<TexturePack> texturePack) {

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
		this->readTexture(texturePage);
	}

	uint32_t textureSize = 0;

	if (this->version == TexturePackVersion::V2) {
		binaryShiftReader->readUint32(&textureSize);
	} else {
		auto position = binaryShiftReader->findBytesSequencePosition(deadBeef);

		textureSize = position - binaryShiftReader->getBuffer()->begin();
		textureSize -= binaryShiftReader->getOffset();
	}

	auto offset = binaryShiftReader->getOffset();

	auto buffer = binaryShiftReader->getBuffer();

	texturePage->start = buffer->begin() + offset;
	texturePage->end = buffer->begin() + offset + textureSize;

	binaryShiftReader->readBytes(nullptr, textureSize);

	if (this->version == TexturePackVersion::V1) {
		binaryShiftReader->readInt32(nullptr);
	}

	std::shared_ptr<vector<uchar>> ucharBuffer =
		std::make_shared<vector<uchar>>();

	std::transform(texturePage->start, texturePage->end,
				   std::back_inserter(*ucharBuffer),
				   [](byte b) { return static_cast<uchar>(b); });

	std::shared_ptr<cv::Mat> image = std::make_shared<cv::Mat>(
		cv::imdecode(*ucharBuffer, cv::IMREAD_UNCHANGED));

	texturePage->image = image;

	for (const auto &texture : texturePage->getTextures()) {
		texture->render(image);
	}

	texturePack->pages.push_back(texturePage);
}

void TexturePackReader::readTexture(std::shared_ptr<TexturePage> texturePage) {

	std::shared_ptr<Texture> texture = std::make_shared<Texture>();

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
