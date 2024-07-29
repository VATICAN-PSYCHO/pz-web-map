#include "file_system.hpp"
#include "logger.hpp"

#include <filesystem>

bool FileSystem::validatePath(std::string unsafePath) {
	std::filesystem::path path(unsafePath);
	return validatePath(path);
}

bool FileSystem::validatePath(std::filesystem::path unsafePath) {
	return std::filesystem::exists(unsafePath);
}

bool FileSystem::isDirectory(std::string unsafePath) {
	std::filesystem::path path(unsafePath);
	return isDirectory(path);
}

bool FileSystem::isDirectory(std::filesystem::path unsafePath) {
	return std::filesystem::is_directory(unsafePath);
}

bool FileSystem::isFile(std::string unsafePath) {
	std::filesystem::path path(unsafePath);
	return isFile(path);
}

bool FileSystem::isFile(std::filesystem::path unsafePath) {
	return std::filesystem::is_regular_file(unsafePath);
}

bool FileSystem::isNonEmptyDirectory(std::string unsafePath) {
	std::filesystem::path path(unsafePath);
	return isNonEmptyDirectory(path);
}

bool FileSystem::isNonEmptyDirectory(std::filesystem::path unsafePath) {
	if (!validatePath(unsafePath)) {
		return false;
	}
	return std::filesystem::directory_iterator(unsafePath) !=
		   std::filesystem::directory_iterator();
}