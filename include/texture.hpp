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

	void setXCord(int32_t xCord);
	int32_t getXCord();

	void setYCord(int32_t yCord);
	int32_t getYCord();

	void setWidth(int32_t width);
	int32_t getWidth();

	void setHeight(int32_t height);
	int32_t getHeight();

	void setXCordOffset(int32_t xCordOffset);
	int32_t getXCordOffset();

	void setYOffset(int32_t yCordOffset);
	int32_t getYOffset();

	void setWidthOffset(int32_t widthOffset);
	int32_t getWidthOffset();

	void setHeightOffset(int32_t heightOffset);
	int32_t getHeightOffset();

	string name;
	int32_t xCord;
	int32_t yCord;
	int32_t width;
	int32_t height;
	int32_t xCordOffset;
	int32_t yCordOffset;
	int32_t widthOffset;
	int32_t heightOffset;
};

#endif