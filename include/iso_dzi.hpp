#ifndef ISO_DZI_HPP
#define ISO_DZI_HPP

#include <cstdint>

struct Point {
	std::int32_t x;
	std::int32_t y;
};

class IsoDZI {
  public:
	IsoDZI();
	~IsoDZI();
	void render();

	static Point getCenter(std::int32_t x, std::int32_t y);

  private:
	static const int32_t SQUARE_HEIGHT = 64;
	static const int32_t SQUARE_WIDTH = 128;
};

#endif
