#include "include/texture.hpp"

Texture::Texture() {}
Texture::~Texture() {}

void Texture::setName(string name) { this->name = name; }

string Texture::getName() { return this->name; }

void Texture::setX(uint32_t x) { this->x = x; }

uint32_t Texture::getX() { return this->x; }

void Texture::setY(uint32_t y) { this->y = y; }

uint32_t Texture::getY() { return this->y; }

void Texture::setW(uint32_t w) { this->w = w; }

uint32_t Texture::getW() { return this->w; }

void Texture::setH(uint32_t h) { this->h = h; }

uint32_t Texture::getH() { return this->h; }

void Texture::setOx(uint32_t ox) { this->ox = ox; }

uint32_t Texture::getOx() { return this->ox; }

void Texture::setOy(uint32_t oy) { this->oy = oy; }

uint32_t Texture::getOy() { return this->oy; }

void Texture::setOw(uint32_t ow) { this->ow = ow; }

uint32_t Texture::getOw() { return this->ow; }

void Texture::setOh(uint32_t oh) { this->oh = oh; }

uint32_t Texture::getOh() { return this->oh; }
