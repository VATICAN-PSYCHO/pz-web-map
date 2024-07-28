#ifndef BINARY_SHIFT_READER_HPP
#define BINARY_SHIFT_READER_HPP

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <deque>
#include <memory>
#include <vector>

using std::byte;
using std::deque;
using std::vector;

class BinaryShiftReader {
  public:
	BinaryShiftReader(std::shared_ptr<vector<byte>> data);
	~BinaryShiftReader();

  public:
	void readUint8(uint8_t *value);
	void readUint16(uint8_t *value);
	void readUint32(uint32_t *value);
	void readUint64(uint64_t *value);

	void readInt8(int8_t *value);
	void readInt16(int16_t *value);
	void readInt32(int32_t *value);
	void readInt64(int64_t *value);

	void readChars(char *value, std::size_t size);

	std::size_t getOffset() { return this->offset; }

  private:
	std::shared_ptr<vector<byte>> data;
	std::size_t offset;
	void copyMemory(byte *dest, std::size_t size);
};

#endif