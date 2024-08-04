#include "renderer.hpp"

Renderer::Renderer(
	std::unordered_map<std::string, std::shared_ptr<Texture>> textures)
	: textures(textures) {}

Renderer::~Renderer() {}

void Renderer::render() {}