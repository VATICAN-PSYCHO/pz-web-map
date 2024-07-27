#include "include/binary_shift_reader.hpp"

BinaryShiftReader::BinaryShiftReader(vector<byte> *data, std::size_t *offset) {
	this->data = data;
	this->offset = offset;
}

BinaryShiftReader::~BinaryShiftReader() { this->data = nullptr; }

void BinaryShiftReader::read_uint8(uint8_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), sizeof(uint8_t));
}

void BinaryShiftReader::read_uint16(uint8_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), sizeof(uint8_t));
}

void BinaryShiftReader::read_uint32(uint32_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), sizeof(uint32_t));
}

void BinaryShiftReader::read_uint64(uint64_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), sizeof(uint64_t));
}

void BinaryShiftReader::read_int8(int8_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), sizeof(int8_t));
}

void BinaryShiftReader::read_int16(int16_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), sizeof(int16_t));
}

void BinaryShiftReader::read_int32(int32_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), sizeof(int32_t));
}

void BinaryShiftReader::read_int64(int64_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), sizeof(int64_t));
}

void BinaryShiftReader::read_chars(char *value, std::size_t size) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), size);
}

void BinaryShiftReader::safe_copy_memory(byte *dest, std::size_t size) {

	if (dest != nullptr)
		std::memcpy(dest, this->data->data() + *this->offset, size);

	*this->offset += size;
}
