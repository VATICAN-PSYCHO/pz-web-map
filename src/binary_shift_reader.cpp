#include "binary_shift_reader.hpp"

#include <memory>

BinaryShiftReader::BinaryShiftReader(
	std::shared_ptr<std::vector<std::byte>> data) {
	this->data = data;
	this->offset = 0;
}

BinaryShiftReader::~BinaryShiftReader() { this->data = nullptr; }

void BinaryShiftReader::readUint8(uint8_t *value) {

	this->copyMemory(reinterpret_cast<byte *>(value), sizeof(uint8_t));
}

void BinaryShiftReader::readUint16(uint8_t *value) {

	this->copyMemory(reinterpret_cast<byte *>(value), sizeof(uint8_t));
}

void BinaryShiftReader::readUint32(uint32_t *value) {

	this->copyMemory(reinterpret_cast<byte *>(value), sizeof(uint32_t));
}

void BinaryShiftReader::readUint64(uint64_t *value) {

	this->copyMemory(reinterpret_cast<byte *>(value), sizeof(uint64_t));
}

void BinaryShiftReader::readInt8(int8_t *value) {

	this->copyMemory(reinterpret_cast<byte *>(value), sizeof(int8_t));
}

void BinaryShiftReader::readInt16(int16_t *value) {

	this->copyMemory(reinterpret_cast<byte *>(value), sizeof(int16_t));
}

void BinaryShiftReader::readInt32(int32_t *value) {

	this->copyMemory(reinterpret_cast<byte *>(value), sizeof(int32_t));
}

void BinaryShiftReader::readInt64(int64_t *value) {

	this->copyMemory(reinterpret_cast<byte *>(value), sizeof(int64_t));
}

void BinaryShiftReader::readChars(char *value, std::size_t size) {

	this->copyMemory(reinterpret_cast<byte *>(value), size);
}

void BinaryShiftReader::copyMemory(byte *dest, std::size_t size) {

	if (dest != nullptr)
		std::memcpy(dest, this->data->data() + this->offset, size);

	this->offset += size;
}
