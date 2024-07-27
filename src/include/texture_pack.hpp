#ifndef TEXTURE_PACK_HPP
#define TEXTURE_PACK_HPP

#include <cstdint>
#include <string>

using std::string;

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

	string name;
	string path;
	uint32_t size;
	bool alpha;
};

#endif