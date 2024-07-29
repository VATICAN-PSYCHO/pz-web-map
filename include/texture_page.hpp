#ifndef TEXTURE_PAGE_HPP
#define TEXTURE_PAGE_HPP

#include "texture.hpp"

#include <memory>
#include <string>
#include <vector>

class TexturePage {
  public:
	TexturePage();
	~TexturePage();

	void setName(std::string name);
	std::string getName();

	void setAlpha(bool alpha);
	bool getAlpha();

	void addTexture(std::shared_ptr<Texture> texture);
	std::shared_ptr<Texture> getTexture(std::size_t index);
	std::vector<std::shared_ptr<Texture>> getTextures();

  private:
	std::string name;
	bool alpha;

  public:
	std::vector<std::shared_ptr<std::byte>> page;
	std::vector<std::shared_ptr<Texture>> textures;
};

#endif