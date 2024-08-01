#include "texture.hpp"

Texture::Texture() {}
Texture::~Texture() {}

void Texture::setName(std::string name) { this->name = name; }

std::string Texture::getName() { return this->name; }

void Texture::setXCord(std::int32_t xCord) { this->xCord = xCord; }

std::int32_t Texture::getXCord() { return this->xCord; }

void Texture::setYCord(std::int32_t yCord) { this->yCord = yCord; }

std::int32_t Texture::getYCord() { return this->yCord; }

void Texture::setWidth(std::int32_t width) { this->width = width; }

std::int32_t Texture::getWidth() { return this->width; }

void Texture::setHeight(std::int32_t height) { this->height = height; }

std::int32_t Texture::getHeight() { return this->height; }

void Texture::setXCordOffset(std::int32_t xCordOffset) {
	this->xCordOffset = xCordOffset;
}

std::int32_t Texture::getXCordOffset() { return this->xCordOffset; }

void Texture::setYOffset(std::int32_t yCordOffset) {
	this->yCordOffset = yCordOffset;
}

std::int32_t Texture::getYCordOffset() { return this->yCordOffset; }

void Texture::setWidthOffset(std::int32_t widthOffset) {
	this->widthOffset = widthOffset;
}

std::int32_t Texture::getWidthOffset() { return this->widthOffset; }

void Texture::setHeightOffset(std::int32_t heightOffset) {
	this->heightOffset = heightOffset;
}

std::int32_t Texture::getHeightOffset() { return this->heightOffset; }

std::shared_ptr<cv::Mat> Texture::render(std::shared_ptr<cv::Mat> image) {

	cv::Rect roi(this->xCord, this->yCord, this->width, this->height);

	cv::Mat texture = (*image)(roi);

	return std::make_shared<cv::Mat>(texture);
}