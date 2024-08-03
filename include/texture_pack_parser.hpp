#ifndef TEXTURE_PACK_PARSER_HPP
#define TEXTURE_PACK_PARSER_HPP

#include <cstdint>
#include <filesystem>
#include <vector>

#include "binary_shift_reader.hpp"
#include "texture_pack.hpp"
#include "thread_pool.hpp"

enum TexturePackVersion { Unknown, V1, V2 };

class TexturePackParser {
  public:
	TexturePackParser(const std::string &path);
	~TexturePackParser();

	std::shared_ptr<TexturePack> parseTexturePack();

	void parsePage(std::shared_ptr<TexturePack> texturePack);
	void parseTexture(std::shared_ptr<TexturePage> texturePage);

  private:
	void determineVersion();

  private:
	std::string path;
	TexturePackVersion version;
	std::shared_ptr<BinaryShiftReader> binaryShiftReader;
	std::shared_ptr<ThreadPool> threadPool;
};

#endif