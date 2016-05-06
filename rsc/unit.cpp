#include "csv_tool.hpp"

#include <iostream>

int main(int argc, char**) {
	CSVObject<6> csv = readCSV<6>("base_set.csv",';');

	for (auto& it : csv) {
		//#name;type;cost;text;power;durability
		std::cout << it[0] << "\t" << it[1] << "\t" << it[2] << std::endl;
	}
	return 0;
}