#ifndef TEXTURE_LIBRARY_HPP
#define TEXTURE_LIBRARY_HPP

#include <memory>

#include "config.hpp"
#include "texture.hpp"
#include "texture_pack.hpp"
#include "texture_pack_reader.hpp"
#include "texture_page.hpp"

class TextureLibrary {
  public:
	TextureLibrary(Config *config);
	~TextureLibrary();

	void proccessTexturePacks();
	void addGameTextures();

	void addTexturePackPath(std::filesystem::path texturePackPath);

	std::shared_ptr<Texture> getTexture(std::string textureName);
	void addTexture(std::string textureName, std::shared_ptr<Texture> texture);

  public:
  private:
	Config *config;
	std::deque<std::filesystem::path> texturePackPaths;
	std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
};

#endif