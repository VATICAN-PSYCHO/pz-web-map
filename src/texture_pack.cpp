#include "texture_pack.hpp"
#include "texture.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

using std::string;

TexturePack::TexturePack() {}

TexturePack::~TexturePack() {}

void TexturePack::setName(string name) { this->name = name; }

string TexturePack::getName() { return this->name; }

void TexturePack::setPath(string path) { this->path = path; }

string TexturePack::getPath() { return this->path; }

void TexturePack::setSize(uint32_t size) { this->size = size; }

uint32_t TexturePack::getSize() { return this->size; }

void TexturePack::setAlpha(bool alpha) { this->alpha = alpha; }

bool TexturePack::getAlpha() { return this->alpha; }

void TexturePack::addTexture(std::shared_ptr<Texture> texture) {
	this->textures.push_back(texture);
}

std::shared_ptr<Texture> TexturePack::getTexture(uint32_t index) {
	return this->textures[index];
}

std::vector<std::shared_ptr<Texture>> TexturePack::getTextures() {
	return this->textures;
}

void TexturePack::setBuffer(std::shared_ptr<std::vector<std::byte>> buffer) {
	this->imageBuffer = buffer;
}

std::shared_ptr<std::vector<std::byte>> TexturePack::getBuffer() {
	return this->imageBuffer;
}