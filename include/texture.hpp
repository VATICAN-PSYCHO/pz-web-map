#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <cstdint>
#include <memory>
#include <opencv2/opencv.hpp>
#include <string>

class Texture {
  public:
	Texture();
	~Texture();

	void setName(std::string name);
	std::string getName();

	void setXCord(std::int32_t xCord);
	std::int32_t getXCord();

	void setYCord(std::int32_t yCord);
	std::int32_t getYCord();

	void setWidth(std::int32_t width);
	std::int32_t getWidth();

	void setHeight(std::int32_t height);
	std::int32_t getHeight();

	void setXCordOffset(std::int32_t xCordOffset);
	std::int32_t getXCordOffset();

	void setYOffset(std::int32_t yCordOffset);
	std::int32_t getYCordOffset();

	void setWidthOffset(std::int32_t widthOffset);
	std::int32_t getWidthOffset();

	void setHeightOffset(std::int32_t heightOffset);
	std::int32_t getHeightOffset();

	void render(std::shared_ptr<cv::Mat>);

	std::string name;
	std::int32_t xCord;
	std::int32_t yCord;
	std::int32_t width;
	std::int32_t height;
	std::int32_t xCordOffset;
	std::int32_t yCordOffset;
	std::int32_t widthOffset;
	std::int32_t heightOffset;

  private:
	std::shared_ptr<cv::Mat> image;
};

#endif