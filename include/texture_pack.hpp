#ifndef TEXTURE_PACK_HPP
#define TEXTURE_PACK_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "texture.hpp"
#include "texture_page.hpp"

using std::string;
using std::vector;

class TexturePack {
  public:
	TexturePack();
	~TexturePack();

	void setName(string name);
	string getName();

	void setPath(string path);
	string getPath();

	// void addTexture(std::shared_ptr<Texture> texture);
	// std::shared_ptr<Texture> getTexture(uint32_t index);
	// std::vector<std::shared_ptr<Texture>> getTextures();

	void addPage(std::shared_ptr<TexturePage> page);
	std::shared_ptr<TexturePage> getPage(uint32_t index);
	std::vector<std::shared_ptr<TexturePage>> getPages();

	void setBuffer(std::shared_ptr<std::vector<std::byte>> buffer);
	std::shared_ptr<std::vector<std::byte>> getBuffer();

	string name;
	string path;

	std::shared_ptr<std::vector<std::byte>> imageBuffer;
	std::vector<std::shared_ptr<TexturePage>> pages;
};

#endif