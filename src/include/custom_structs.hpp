#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

struct TexturePack {
	string name;
	// string path;
	uint32_t size;
	bool alpha;
};

struct Texture {
	string name;
	vector<byte> data;
	uint32_t x;
	uint32_t y;
	uint32_t w;
	uint32_t h;
	uint32_t ox;
	uint32_t oy;
	uint32_t ow;
	uint32_t oh;
};
