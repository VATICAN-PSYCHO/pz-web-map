#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>
#include <string>
#include <unordered_map>

#include "texture.hpp"

class Renderer {
  public:
	Renderer(
		std::unordered_map<std::string, std::shared_ptr<Texture>> textures);
	~Renderer();
	void render();

  private:
	std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
};

#endif