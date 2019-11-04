#pragma once
#include <iostream>
#include <filesystem>
#include <regex>
#include <vector>

class IOUtils
{
public:
	static std::pair<std::string, std::string> getNameIndex(const std::string& fileName);
	static bool file_exists(const std::string& name);
	static bool getFilePathFromUser(const std::string& message, std::vector<std::string>& paths);
	static bool getFilesFromDirectory(const std::string& dirPath, std::vector<std::string>& paths);
	static bool saveMatchingFactorsToFile(
		const std::vector<std::vector<double>>& factors,
		const std::vector<std::string>& templateFiles,
		const std::vector<std::string>& inputFiles);
	static std::string extractDirectory(const std::string& filePath);
	static std::string extractFileName(const std::string& filePath);
	static std::string cutWavExtension(const std::string& text);
	static void pressEnterToContinue();
	static void pressEnterToContinue(const std::string & message);
private:
	static std::vector<std::filesystem::path> file_list(std::filesystem::path dir, std::regex ext_pattern);
	static std::string toRegex(const std::string& text);
};

