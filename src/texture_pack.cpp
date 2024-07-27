#include "include/texture_pack.hpp"

TexturePack::TexturePack() {}

TexturePack::~TexturePack() {}

void TexturePack::setName(string name) { this->name = name; }

string TexturePack::getName() { return this->name; }

void TexturePack::setPath(string path) { this->path = path; }

string TexturePack::getPath() { return this->path; }

void TexturePack::setSize(uint32_t size) { this->size = size; }

uint32_t TexturePack::getSize() { return this->size; }

void TexturePack::setAlpha(bool alpha) { this->alpha = alpha; }

bool TexturePack::getAlpha() { return this->alpha; }
