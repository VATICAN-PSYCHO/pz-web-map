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
	BinaryShiftReader(deque<byte> *data);
	~BinaryShiftReader();

  public:
	void read_uint8(uint8_t *value);
	void read_uint16(uint16_t *value);
	void read_uint32(uint32_t *value);
	void read_uint64(uint64_t *value);

	void read_int8(int8_t *value);
	void read_int16(int16_t *value);
	void read_int32(int32_t *value);
	void read_int64(int64_t *value);

	void read_bytes_vector(vector<byte> *value, std::size_t size);

  private:
	deque<byte> *binaryData;
	void safe_copy_memory(byte *dest, std::deque<byte>::iterator,
						  std::size_t size);
};

#endif