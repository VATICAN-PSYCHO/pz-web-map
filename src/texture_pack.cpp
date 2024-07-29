#include "texture_pack.hpp"
#include "texture.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

using std::string;

TexturePack::TexturePack() {
	this->path = "";
	this->pages = std::vector<std::shared_ptr<TexturePage>>();
}

TexturePack::~TexturePack() {}

void TexturePack::setPath(string path) { this->path = path; }

string TexturePack::getPath() { return this->path; }

void TexturePack::addPage(std::shared_ptr<TexturePage> page) {
	this->pages.push_back(page);
}

std::shared_ptr<TexturePage> TexturePack::getPage(uint32_t index) {
	return this->pages[index];
}

std::vector<std::shared_ptr<TexturePage>> TexturePack::getPages() {
	return this->pages;
}
