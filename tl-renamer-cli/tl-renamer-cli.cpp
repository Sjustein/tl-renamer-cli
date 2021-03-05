#include <iostream>
#include <string>
#include <filesystem>
#include <map>
#include <stdio.h>

namespace fs = std::filesystem;

#ifdef _WIN32
	#define PATH_SEP '\\'
#else
	#define PATH_SEP '/'
#endif

void processDirectory(std::string dir)
{
	std::map<unsigned long, std::string> files;

	// Gather the file creation time of files
	for (const auto& file : fs::directory_iterator(dir)) {
		if (!file.is_directory() && file.is_regular_file() && !file.is_symlink()) {
			unsigned long millis = file.last_write_time().time_since_epoch() / std::chrono::milliseconds(1);
			files.emplace(millis, file.path().filename().string());
		}
	}

	// The dictionary is already ordered, check if the converted directory exists
	fs::directory_entry convertedPath(dir + "Processed");
	if (convertedPath.exists())
		std::filesystem::remove_all(convertedPath.path());

	std::filesystem::create_directory(convertedPath.path());

	int i = 0;
	std::string dirBase = dir + "Processed" + PATH_SEP;
	for (const auto& cur : files) {
		fs::directory_entry curFile(dir + cur.second);
		std::string curFilePath = dirBase + std::to_string(i) + curFile.path().extension().string();
		fs::directory_entry toPath(curFilePath);

		std::filesystem::copy(curFile.path(), toPath.path());
		std::cout << cur.second << " -> " << curFilePath << std::endl;

		i++;
	}
}

bool validateArgs(int argc, char* argv[])
{
	return false;
	if (argc < 3)
		return true;

	return argv[2] != "-m";
}

void printHelp() {
	std::cout << "USAGE: tl-renamer-cli path_to_proces [arguments]" << std::endl <<std::endl
				<< "-m " << std::setw(75) << "Move the files to the processed directory, instead of copying them" << std::endl;
}

int main(int argc, char *argv[])
{
	// Check if the arguments are valid
	if (!validateArgs(argc, argv)) {
		std::cout << "The specified command line arguments are not in the correct format." << std::endl << std::endl;
		printHelp();
	}

	// Check if the directory has been specified in a cli argument
	std::string dir;
	if (argc > 1)
		dir = argv[1];
	else {
		std::cout << "Please input the path to the directory with the files to be renamed] ";
		std::getline(std::cin, dir, '\n');
	}
		
	// Check if the last character is a path seperator. Otherwise, add it
	if (dir.back() != PATH_SEP)
		dir += PATH_SEP;

	processDirectory(dir);
	return 0;
}
