#include "iso_dzi.hpp"

IsoDZI::IsoDZI() {}

IsoDZI::~IsoDZI() {}

void IsoDZI::render() {}

Point IsoDZI::getCenter(std::int32_t x, std::int32_t y) {
	Point p;
	p.x = x * SQUARE_WIDTH;
	p.y = y * SQUARE_HEIGHT;
	return p;
}