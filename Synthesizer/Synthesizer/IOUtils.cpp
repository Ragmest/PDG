#include "IOUtils.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <type_traits>

namespace fs = std::filesystem;

std::pair<std::string, std::string> IOUtils::getNameIndex(const std::string& fileName)
{
	auto nameIndex = IOUtils::cutWavExtension(fileName);
	std::size_t pos = nameIndex.find('_');
	auto name = nameIndex.substr(0, pos);
	auto index = nameIndex.substr(pos + 1);
	return std::make_pair(name, index);
};


std::vector<fs::path> IOUtils::file_list(fs::path dir, std::regex ext_pattern)
{
	std::vector<fs::path> result;

	using iterator = std::conditional< false,
		fs::recursive_directory_iterator, fs::directory_iterator >::type;

	const iterator end;
	for (iterator iter{ dir }; iter != end; ++iter)
	{
		const std::string extension = iter->path().extension().string();
		if (fs::is_regular_file(*iter) && std::regex_match(extension, ext_pattern)) result.push_back(*iter);
	}

	return result;
}

std::string IOUtils::extractDirectory(const std::string & filePath)
{
	std::size_t pos = filePath.rfind('\\');
	return filePath.substr(0, pos + 1);
}

std::string IOUtils::extractFileName(const std::string & filePath)
{
	std::size_t pos = filePath.rfind('\\');
	return filePath.substr(pos + 1);
}

std::string IOUtils::toRegex(const std::string & text)
{
	std::string regex = text;
	std::regex dot{ "\\." };
	std::regex star{ "\\*" };

	regex = std::regex_replace(regex, dot, "\\\.");
	regex = std::regex_replace(regex, star, "\\.*");

	return regex;
}

std::string IOUtils::cutWavExtension(const std::string & text)
{
	std::size_t pos = text.rfind('.');
	return text.substr(0, pos);
}

bool IOUtils::file_exists(const std::string& name) {
	std::ifstream f(name.c_str());
	return f.good();
}

bool IOUtils::getFilePathFromUser(const std::string& message, std::vector<std::string>& paths)
{
	bool fileExists = false;
	std::string path{ "" };
	do
	{
		std::cout << message << std::endl;
		std::cin >> path;
		auto fileName = extractFileName(path);
		auto pos = fileName.find('*');
		if (fileName.find('*') == std::string::npos)
		{
			fileExists = file_exists(path);
			if (path == "end")
				return false;
			if (!fileExists)
				std::cout << "File " << path << " doesn't exists. Try again or write 'end' to terminate" << std::endl;
			else
				paths.push_back(path);
		}
		else
		{
			auto fileDirectory = extractDirectory(path);

			for (const auto& file_path : file_list(fileDirectory, std::regex("\\.wav")))
			{
				fileExists = file_exists(file_path.string());
				if (!fileExists)
					std::cout << "File " << file_path.string() << " doesn't exists. Try again or write 'end' to terminate" << std::endl;
				else
				{
					paths.push_back(file_path.string());
				}
			}
		}

	} while (!fileExists);

	return true;
}

bool IOUtils::getFilesFromDirectory(const std::string& dirPath, std::vector<std::string>& paths)
{
	for (const auto& file_path : file_list(dirPath, std::regex("\\.wav")))
	{
		bool fileExists = file_exists(file_path.string());
		if (!fileExists)
			std::cout << "File " << file_path.string() << " doesn't exists. Try again or write 'end' to terminate" << std::endl;
		else
		{
			paths.push_back(file_path.string());
		}
	}

	return paths.size() != 0;
}

bool IOUtils::saveMatchingFactorsToFile(
	const std::vector<std::vector<double>>& factors,
	const std::vector<std::string>& templateFiles,
	const std::vector<std::string>& inputFiles)
{
	const int width(12);

	std::ofstream os;

	os.open("./Matching_Factors.txt");
	if (!os.is_open())
		return false;

	os << std::setw(width) << " ";

	for (const auto& inputFile : inputFiles)
	{
		auto name = extractFileName(inputFile);
		name = cutWavExtension(name);
		os << std::setw(width) << std::left << name;
	}

	os << "\r\n";

	for (uint32_t i = 0; i < templateFiles.size(); i++)
	{
		auto name = extractFileName(templateFiles[i]);
		name = cutWavExtension(name);
		os << std::setw(width) << std::left << name;

		for (uint32_t j = 0; j < factors[i].size(); j++)
		{
			os << std::setw(width) << std::right << std::to_string(factors[i][j]);
		}

		os << "\r\n";
	}

	os.close();

	return true;
}

void IOUtils::pressEnterToContinue()
{
	pressEnterToContinue("Press ENTER to continue...");
}

void IOUtils::pressEnterToContinue(const std::string & message)
{
	std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
	std::cout << message << std::endl << std::flush;
	std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
}
