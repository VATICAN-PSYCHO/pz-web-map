#ifndef FILE_SYSTEM_HPP
#define FILE_SYSTEM_HPP

#include <filesystem>
#include <string>
#include <vector>

class FileSystem {
  public:
	static bool validatePath(std::string unsafePath);
	static bool validatePath(std::filesystem::path unsafePath);

	static bool isDirectory(std::string unsafePath);
	static bool isDirectory(std::filesystem::path unsafePath);

	static bool isFile(std::string unsafePath);
	static bool isFile(std::filesystem::path unsafePath);

	static bool isNonEmptyDirectory(std::string unsafePath);
	static bool isNonEmptyDirectory(std::filesystem::path unsafePath);
};

#endif