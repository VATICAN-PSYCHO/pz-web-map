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

	void setPath(string path);
	string getPath();

	void addPage(std::shared_ptr<TexturePage> page);
	std::shared_ptr<TexturePage> getPage(uint32_t index);
	std::vector<std::shared_ptr<TexturePage>> getPages();

	string path;
	std::vector<std::shared_ptr<TexturePage>> pages;
	std::shared_ptr<std::vector<std::byte>> pageBuffer;
};

#endif