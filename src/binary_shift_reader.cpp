#include "binary_shift_reader.hpp"

#include <memory>
#include <stdexcept>

BinaryShiftReader::BinaryShiftReader(
	std::shared_ptr<std::vector<std::byte>> buffer) {

	this->buffer = buffer;
	this->offset = 0;
}

BinaryShiftReader::~BinaryShiftReader() { this->buffer = nullptr; }

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

void BinaryShiftReader::readBytes(byte *value, std::size_t size) {

	this->copyMemory(value, size);
}

void BinaryShiftReader::readChars(char *value, std::size_t size) {

	this->copyMemory(reinterpret_cast<byte *>(value), size);
}

std::size_t BinaryShiftReader::getOffset() { return this->offset; }

std::size_t BinaryShiftReader::getSize() { return this->buffer->size(); }

std::byte BinaryShiftReader::at(std::size_t index) {

	if (index >= this->buffer->size())
		throw std::out_of_range("Index out of range");

	return this->buffer->at(index);
}

std::shared_ptr<std::vector<std::byte>> BinaryShiftReader::getBuffer() {

	return this->buffer;
}

void BinaryShiftReader::copyMemory(byte *dest, std::size_t size) {

	if (dest != nullptr)
		std::memcpy(dest, this->buffer->data() + this->offset, size);

	this->offset += size;
}
