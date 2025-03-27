#include "json.hpp"
	
#include <iostream>
#include <iomanip>
#include <sstream>



int main() {
	std::cout << "Your input:\n";
	std::string input;
	std::getline(std::cin, input);

	std::stringstream stream(input);
	json::JsonInput in(&stream);

	json::JsonNumber number(0);
	number.read(in);

	std::cout << "-- Number parsing result --\n";
	std::cout << "Value: " << std::setprecision(200) << std::fixed << number.value << '\n';
	std::cout << "Dec: " << number.decimal << '\n';
	std::cout << "Exp: " << number.exp << '\n';

	return 0;
}