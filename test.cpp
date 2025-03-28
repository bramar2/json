#include "json.hpp"
#include "types.hpp"
	
#include <iostream>
#include <sstream>



int main() {
	std::cout << "Your input:\n";
	std::string input;
	std::getline(std::cin, input);

	std::stringstream stream(input);
	json::JsonInput in(&stream);

	json::JsonString obj;
	bool res = obj.read(in);

	std::cout << "-- Parsing result --\n";
	std::cout << "Success: " << std::boolalpha << res << '\n';
	std::cout << "Value: " << obj.value << '\n';
	std::cout << "-- Writing result --\n";
	std::cout << "Result: ";
	res = obj.write(std::cout);
	std::cout << "\nSuccess: " << std::boolalpha << res << '\n';

	return 0;
}