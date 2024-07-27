#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <cstdint>
#include <string>

using std::string;

class Texture {
  public:
	Texture();
	~Texture();

	void setName(string name);
	string getName();

	void setX(uint32_t x);
	uint32_t getX();

	void setY(uint32_t y);
	uint32_t getY();

	void setW(uint32_t w);
	uint32_t getW();

	void setH(uint32_t h);
	uint32_t getH();

	void setOx(uint32_t ox);
	uint32_t getOx();

	void setOy(uint32_t oy);
	uint32_t getOy();

	void setOw(uint32_t ow);
	uint32_t getOw();

	void setOh(uint32_t oh);
	uint32_t getOh();

	string name;
	uint32_t x;
	uint32_t y;
	uint32_t w;
	uint32_t h;
	uint32_t ox;
	uint32_t oy;
	uint32_t ow;
	uint32_t oh;
};

#endif