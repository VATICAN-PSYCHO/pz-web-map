#ifndef TEXTURE_PACK_HPP
#define TEXTURE_PACK_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "texture.hpp"

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

	void setSize(uint32_t size);
	uint32_t getSize();

	void setAlpha(bool alpha);
	bool getAlpha();

	void addTexture(std::shared_ptr<Texture> texture);
	std::shared_ptr<Texture> getTexture(uint32_t index);
	std::vector<std::shared_ptr<Texture>> getTextures();

	void setBuffer(std::shared_ptr<std::vector<std::byte>> buffer);
	std::shared_ptr<std::vector<std::byte>> getBuffer();

	string name;
	string path;
	uint32_t size;
	bool alpha;

	std::shared_ptr<std::vector<std::byte>> imageBuffer;
	std::vector<std::shared_ptr<Texture>> textures;
};

#endif