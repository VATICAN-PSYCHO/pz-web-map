#include "binary_shift_reader.hpp"

BinaryShiftReader::BinaryShiftReader(deque<byte> *data) {
	this->binaryData = data;
}

BinaryShiftReader::~BinaryShiftReader() { this->binaryData = nullptr; }

void BinaryShiftReader::read_uint8(uint8_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), binaryData->begin(),
						   sizeof(uint8_t));

	binaryData->erase(binaryData->begin(), binaryData->begin() + 1);
}

void BinaryShiftReader::read_uint16(uint16_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), binaryData->begin(),
						   sizeof(uint16_t));

	binaryData->erase(binaryData->begin(), binaryData->begin() + 2);
}

void BinaryShiftReader::read_uint32(uint32_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), binaryData->begin(),
						   sizeof(uint32_t));

	binaryData->erase(binaryData->begin(), binaryData->begin() + 4);
}

void BinaryShiftReader::read_uint64(uint64_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), binaryData->begin(),
						   sizeof(uint64_t));

	binaryData->erase(binaryData->begin(), binaryData->begin() + 8);
}

void BinaryShiftReader::read_int8(int8_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), binaryData->begin(),
						   sizeof(int8_t));

	binaryData->erase(binaryData->begin(), binaryData->begin() + 1);
}

void BinaryShiftReader::read_int16(int16_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), binaryData->begin(),
						   sizeof(int16_t));

	binaryData->erase(binaryData->begin(), binaryData->begin() + 2);
}

void BinaryShiftReader::read_int32(int32_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), binaryData->begin(),
						   sizeof(int32_t));

	binaryData->erase(binaryData->begin(), binaryData->begin() + 4);
}

void BinaryShiftReader::read_int64(int64_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), binaryData->begin(),
						   sizeof(int64_t));

	binaryData->erase(binaryData->begin(), binaryData->begin() + 8);
}

void BinaryShiftReader::read_bytes_vector(vector<byte> *value, size_t size) {

	value->insert(value->begin(), binaryData->begin(),
				  binaryData->begin() + size);

	binaryData->erase(binaryData->begin(), binaryData->begin() + size);
}

void BinaryShiftReader::safe_copy_memory(byte *dest,
										 std::deque<byte>::iterator src,
										 size_t size) {
	if (dest != nullptr && std::distance(src, binaryData->end()) >= size)
		memcpy(dest, &(*src), size);
}
