#include <iostream>
#include <string>
#include <filesystem>
#include <map>

namespace fs = std::filesystem;

char getPathSep() {
#ifdef _WIN32
	return '\\';
#else
	return '/';
#endif
}

void processDirectory(std::string dir) {
	std::map<std::string, unsigned long> files;

	// Gather the file creation time of files
	for (const auto& file : fs::directory_iterator(dir)) {
		if (!file.is_directory() && file.is_regular_file() && !file.is_symlink()) {
			unsigned long millis = file.last_write_time().time_since_epoch() / std::chrono::milliseconds(1);
			files.emplace(file.path().filename().string(), millis);
		}
	}

	// Order the directory
}

int main(int argc, char *argv[])
{
	// Find the path seperator for the current operating system
	char pathSep = getPathSep();

	// Check if the directory has been specified in a cli argument
	std::string dir;
	if (argc > 1)
		dir = argv[1];
	else {
		std::cout << "Please input the path to the directory with the files to be renamed] ";
		std::getline(std::cin, dir, '\n');
	}

	// Check if the last character is a path seperator. Otherwise, add it
	if (dir.back() != pathSep)
		dir += pathSep;

	processDirectory(dir);
	return 0;
}
