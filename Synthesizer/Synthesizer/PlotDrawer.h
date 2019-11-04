#pragma once
#include <vector>
#include <string>

class PlotDrawer
{
public:
	void static draw(
		const std::string& labelX,
		const std::string& labelY,
		const std::vector<double>& valuesX,
		const std::vector<double>& valuesY);
	void static draw(const std::vector<std::vector<double>>& matrix);

private:
	template<class T>
	static std::string vectorToString(const std::vector<T>& vec);


};

