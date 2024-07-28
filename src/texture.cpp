#include "texture.hpp"

Texture::Texture() {}
Texture::~Texture() {}

void Texture::setName(string name) { this->name = name; }

string Texture::getName() { return this->name; }

void Texture::setXCord(int32_t xCord) { this->xCord = xCord; }

int32_t Texture::getXCord() { return this->xCord; }

void Texture::setYCord(int32_t yCord) { this->yCord = yCord; }

int32_t Texture::getYCord() { return this->yCord; }

void Texture::setWidth(int32_t width) { this->width = width; }

int32_t Texture::getWidth() { return this->width; }

void Texture::setHeight(int32_t height) { this->height = height; }

int32_t Texture::getHeight() { return this->height; }

void Texture::setXCordOffset(int32_t xCordOffset) {
	this->xCordOffset = xCordOffset;
}

int32_t Texture::getXCordOffset() { return this->xCordOffset; }

void Texture::setYOffset(int32_t yCordOffset) {
	this->yCordOffset = yCordOffset;
}

int32_t Texture::getYOffset() { return this->yCordOffset; }

void Texture::setWidthOffset(int32_t widthOffset) {
	this->widthOffset = widthOffset;
}

int32_t Texture::getWidthOffset() { return this->widthOffset; }

void Texture::setHeightOffset(int32_t heightOffset) {
	this->heightOffset = heightOffset;
}

int32_t Texture::getHeightOffset() { return this->heightOffset; }
