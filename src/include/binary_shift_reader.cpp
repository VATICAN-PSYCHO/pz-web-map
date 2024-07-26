#include "binary_shift_reader.hpp"

using namespace std;

BinaryShiftReader::BinaryShiftReader(vector<byte> *data) {
	this->binaryData = data;
}

BinaryShiftReader::~BinaryShiftReader() { this->binaryData = nullptr; }

void BinaryShiftReader::read_uint8(uint8_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), binaryData->data(),
						   sizeof(uint8_t));

	binaryData->erase(binaryData->begin(), binaryData->begin() + 1);
}

void BinaryShiftReader::read_uint16(uint16_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), binaryData->data(),
						   sizeof(uint16_t));

	binaryData->erase(binaryData->begin(), binaryData->begin() + 2);
}

void BinaryShiftReader::read_uint32(uint32_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), binaryData->data(),
						   sizeof(uint32_t));

	binaryData->erase(binaryData->begin(), binaryData->begin() + 4);
}

void BinaryShiftReader::read_uint64(uint64_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), binaryData->data(),
						   sizeof(uint64_t));

	binaryData->erase(binaryData->begin(), binaryData->begin() + 8);
}

void BinaryShiftReader::read_int8(int8_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), binaryData->data(),
						   sizeof(int8_t));

	binaryData->erase(binaryData->begin(), binaryData->begin() + 1);
}

void BinaryShiftReader::read_int16(int16_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), binaryData->data(),
						   sizeof(int16_t));

	binaryData->erase(binaryData->begin(), binaryData->begin() + 2);
}

void BinaryShiftReader::read_int32(int32_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), binaryData->data(),
						   sizeof(int32_t));

	binaryData->erase(binaryData->begin(), binaryData->begin() + 4);
}

void BinaryShiftReader::read_int64(int64_t *value) {

	this->safe_copy_memory(reinterpret_cast<byte *>(value), binaryData->data(),
						   sizeof(int64_t));

	binaryData->erase(binaryData->begin(), binaryData->begin() + 8);
}

void BinaryShiftReader::read_bytes(vector<byte> *value, size_t size) {

	value->insert(value->begin(), binaryData->begin(),
				  binaryData->begin() + size);

	binaryData->erase(binaryData->begin(), binaryData->begin() + size);
}

void BinaryShiftReader::safe_copy_memory(byte *dest, byte *src, size_t size) {
	if (dest != nullptr && src != nullptr)
		memcpy(dest, src, size);
}