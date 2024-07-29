#include "texture_page.hpp"
// #include "texture.hpp"

#include <memory>
#include <string>
#include <vector>

TexturePage::TexturePage() {
	this->name = "";
	this->alpha = false;
	this->textures = std::vector<std::shared_ptr<Texture>>();
}

TexturePage::~TexturePage() {}

void TexturePage::setName(std::string name) { this->name = name; }

std::string TexturePage::getName() { return this->name; }

void TexturePage::setAlpha(bool alpha) { this->alpha = alpha; }

bool TexturePage::getAlpha() { return this->alpha; }

void TexturePage::addTexture(std::shared_ptr<Texture> texture) {
	this->textures.push_back(texture);
}

std::shared_ptr<Texture> TexturePage::getTexture(std::size_t index) {
	return this->textures[index];
}

std::vector<std::shared_ptr<Texture>> TexturePage::getTextures() {
	return this->textures;
}