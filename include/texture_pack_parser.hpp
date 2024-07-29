#ifndef TEXTURE_PACK_PARSER_HPP
#define TEXTURE_PACK_PARSER_HPP

#include <cstdint>
#include <filesystem>
#include <vector>

#include "binary_shift_reader.hpp"
#include "texture_pack.hpp"

class TexturePackParser {
  public:
	TexturePackParser(const std::string &path);
	~TexturePackParser();

	std::shared_ptr<TexturePack> parse();

	bool needSanitization(std::string sanitizeText);

  private:
	std::string path;
	std::shared_ptr<BinaryShiftReader> binaryShiftReader;
};

#endif