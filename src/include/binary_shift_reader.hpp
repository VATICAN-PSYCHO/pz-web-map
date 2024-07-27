#ifndef BINARY_SHIFT_READER_HPP
#define BINARY_SHIFT_READER_HPP

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <deque>
#include <vector>

using std::byte;
using std::deque;
using std::vector;

class BinaryShiftReader {
  public:
	BinaryShiftReader(vector<byte> *data, std::size_t *offset);
	~BinaryShiftReader();

  public:
	void read_uint8(uint8_t *value);
	void read_uint16(uint8_t *value);
	void read_uint32(uint32_t *value);
	void read_uint64(uint64_t *value);

	void read_int8(int8_t *value);
	void read_int16(int16_t *value);
	void read_int32(int32_t *value);
	void read_int64(int64_t *value);

	void read_chars(char *value, std::size_t size);

  private:
	vector<byte> *data;
	std::size_t *offset;
	void safe_copy_memory(byte *dest, std::size_t size);
};

#endif