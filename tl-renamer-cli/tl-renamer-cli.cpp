#include <iostream>
#include <string>
#include <filesystem>
#include <map>
#include <stdio.h>
#include <algorithm>

namespace fs = std::filesystem;

#ifdef _WIN32
	#define PATH_SEP '\\'
#else
	#define PATH_SEP '/'
#endif

bool MoveFiles = false;
std::string ExtFilter = "*";

void processDirectory(std::string dir)
{
	fs::directory_entry check(dir);

	// Check if this is a directory and it exiists
	if (!check.exists() || !check.is_directory()) {
		std::cout << "The specified path does not exist or is not a directory." << std::endl;
		return;
	}

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

		if (curFile.path().extension().string() == ExtFilter) {
			std::string curFilePath = dirBase + std::to_string(i) + curFile.path().extension().string();
			fs::directory_entry toPath(curFilePath);

			std::filesystem::copy(curFile.path(), toPath.path());
			if (MoveFiles)
				std::filesystem::remove(curFile.path());

			std::cout << cur.second << " -> " << curFilePath << std::endl;

			i++;
		}
		else
			std::cout << "Skipping file " << curFile.path().string() << " because of extension filter: " << ExtFilter << std::endl;
	}

	std::cout << "Finished processing the directory.";
}

void printHelp()
{
	std::cout << "USAGE: tl-renamer-cli path_to_proces [arguments]" << std::endl << std::endl;
	std::printf("%-20s", "-m (--move)");
	std::cout << "Move the files to the processed directory, instead of copying them" << std::endl;
	std::printf("%-20s", "-h (--help)");
	std::cout << "Prints this overview" << std::endl;
	std::printf("%-20s", "-f (--filter)");
	std::cout << "Specify a filter for the file extension to move" << std::endl;
}

std::pair<bool, bool> validateArgs(int argc, char* argv[])
{
	std::string validArgs[4] = { "-m", "--move", "-h", "--help" };
	std::string argsWithArg[2] = { "-f", "--filter" };
	bool firstIndexIsArg = false;

	for (int i = 1; i < argc; i++) {
		// Check if this argument is valid, or if it has an extra argument if it requires that
		std::string curArg = argv[i];
		bool isArgument = std::find(std::begin(validArgs), std::end(validArgs), curArg) != std::end(validArgs);
		bool isArgumentWithArg = std::find(std::begin(argsWithArg), std::end(argsWithArg), curArg) != std::end(argsWithArg);

		if ((!isArgument && !(isArgumentWithArg && argc > i)) && i != 1) {
			std::cout << "Invalid argument detected: " << curArg << std::endl << std::endl;
			return std::make_pair(false, firstIndexIsArg);
		}

		if (i == 1 && (isArgument || (isArgumentWithArg && argc > i)))
			firstIndexIsArg = true;

		// If this is an argument with required argument, skip that argument
		if (isArgumentWithArg)
			i++;
	}

	return std::make_pair(true, firstIndexIsArg);
}

bool processArguments(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++) {
		std::string arg = argv[i];
		if (arg == "-m" || arg == "--move")
			MoveFiles = true;
		else if (arg == "-h" || arg == "--help") {
			printHelp();
			return true;
		}
		if (arg == "-f" || arg == "--filter") {
			std::string nextVal = argv[i + 1];
			ExtFilter = argv[i+1];
			i++;
		}
	}

	return false;
}

int main(int argc, char *argv[])
{
	// Check if the arguments are valid
	std::pair<bool, bool> argValidation = validateArgs(argc, argv);
	if (!argValidation.first) {
		printHelp();
		return 1;
	}

	// Check if there are command line arguments to be checked
	if (argc > 1)
		if (processArguments(argc, argv))
			return 2;

	// Check if the directory has been specified in a cli argument
	std::string dir;
	if (argc > 1 && !argValidation.second)
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
