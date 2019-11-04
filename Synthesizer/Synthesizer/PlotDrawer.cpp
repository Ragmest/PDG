#include "PlotDrawer.h"
#include <iostream>
#include <string>
#include <fstream>
#include <thread>

void PlotDrawer::draw(
	const std::string & labelX,
	const std::string & labelY,
	const std::vector<double> & valuesX,
	const std::vector<double> & valuesY)
{
	std::cout << "Parsing params..." << std::endl;
	std::ofstream parameters("parameters.txt", std::ofstream::out);

	parameters << "regular" << "\n";
	parameters << labelX << "\n";
	parameters << labelY << "\n";
	parameters << vectorToString(valuesX) << "\n";
	parameters << vectorToString(valuesY) << "\n";

	parameters.close();

	std::cout << "Drawing plot..." << std::endl;
	system("python PlotDrawer.py &");

	std::cout << "Plot finished." << std::endl;
}

void PlotDrawer::draw(const std::vector<std::vector<double>>& matrix)
{
	std::cout << "Parsing params..." << std::endl;
	std::ofstream parameters("parameters.txt", std::ofstream::out);

	parameters << "grid" << "\n";
	for (const auto& row : matrix)
		parameters << vectorToString(row) << "\n";

	parameters.close();

	std::cout << "Drawing plot..." << std::endl;
	system("python PlotDrawer.py");

	std::cout << "Plot finished." << std::endl;
}

template<class T>
std::string PlotDrawer::vectorToString(const std::vector<T>& vec)
{
	std::string result{ "" };

	for (uint32_t i = 0; i < vec.size(); i++)
	{
		result += std::to_string(vec[i]);
		result += (i != vec.size() - 1 ? "," : "");
	}
	return result;
}
