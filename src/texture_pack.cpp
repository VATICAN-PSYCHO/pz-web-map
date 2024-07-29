#include "texture_pack.hpp"
#include "texture.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

using std::string;

TexturePack::TexturePack() {
	this->name = "";
	this->path = "";
	this->pages = std::vector<std::shared_ptr<TexturePage>>();
}

TexturePack::~TexturePack() {}

void TexturePack::setName(string name) { this->name = name; }

string TexturePack::getName() { return this->name; }

void TexturePack::setPath(string path) { this->path = path; }

string TexturePack::getPath() { return this->path; }

// void TexturePack::addTexture(std::shared_ptr<Texture> texture) {
// 	this->textures.push_back(texture);
// }

// std::shared_ptr<Texture> TexturePack::getTexture(uint32_t index) {
// 	return this->textures[index];
// }

// std::vector<std::shared_ptr<Texture>> TexturePack::getTextures() {
// 	return this->textures;
// }

void TexturePack::addPage(std::shared_ptr<TexturePage> page) {
	this->pages.push_back(page);
}

std::shared_ptr<TexturePage> TexturePack::getPage(uint32_t index) {
	return this->pages[index];
}

std::vector<std::shared_ptr<TexturePage>> TexturePack::getPages() {
	return this->pages;
}

void TexturePack::setBuffer(std::shared_ptr<std::vector<std::byte>> buffer) {
	this->imageBuffer = buffer;
}

std::shared_ptr<std::vector<std::byte>> TexturePack::getBuffer() {
	return this->imageBuffer;
}