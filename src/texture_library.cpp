#include "texture_library.hpp"

#include <filesystem>
#include <texture_pack_reader.hpp>

TextureLibrary::TextureLibrary(Config *config) {
	this->config = config;
	this->textures =
		std::unordered_map<std::string, std::shared_ptr<Texture>>();
}

TextureLibrary::~TextureLibrary() {}

void TextureLibrary::proccessTexturePacks() {
	this->addGameTextures();

	for (const auto &texturePackPath : this->texturePackPaths) {
		auto texturePackReader =
			std::make_shared<TexturePackReader>(texturePackPath.string());

		auto texturePack = texturePackReader->read();
	}

	for (const auto &texturePackPath : this->texturePackPaths) {
		auto texturePackReader =
			std::make_shared<TexturePackReader>(texturePackPath.string());

		auto texturePack = texturePackReader->read();

		for (const auto &texturePage : texturePack->getPages()) {
			for (const auto &texture : texturePage->getTextures()) {
				this->addTexture(texture->name, texture);
			}
		}
	}
}

void TextureLibrary::addGameTextures() {
	auto gameDir = this->config->directories.gameDir;

	std::filesystem::path texturePacksDirectory =
		std::filesystem::path(gameDir) / "media" / "texturepacks";

	for (const auto &texturePackPath :
		 std::filesystem::directory_iterator(texturePacksDirectory)) {

		if (texturePackPath.is_directory()) {
			continue;
		}

		bool found = false;

		for (const auto &includePackName : this->config->includePacksNames) {

			if (texturePackPath.path().filename().string() == includePackName) {
				found = true;
				break;
			}
		}

		if (!found) {
			continue;
		}

		this->addTexturePackPath(texturePackPath.path());
	}
}

void TextureLibrary::addTexturePackPath(std::filesystem::path texturePackPath) {
	this->texturePackPaths.push_back(texturePackPath);
}

std::shared_ptr<Texture> TextureLibrary::getTexture(std::string textureName) {
	return this->textures[textureName];
}

void TextureLibrary::addTexture(std::string textureName,
								std::shared_ptr<Texture> texture) {
	this->textures[textureName] = texture;
}
