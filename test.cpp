#include "json.hpp"
#include "json_bool.hpp"
#include "json_object.hpp"
	
#include <iostream>
#include <sstream>
#include <utility>

void example(std::istream& in, std::ostream& out) {
	// Reading
	json::JsonInput j_in(&in);
	std::unique_ptr<json::JsonElement> json = json::parse(j_in);
	if (!json) {
		throw std::runtime_error("malformed json");
	}
	switch (json->type()) {
	case json::JsonType::Null:
		std::cout << "Received null\n";
		break;
	case json::JsonType::Bool:
		{
			json::JsonBool* jsonBool = dynamic_cast<json::JsonBool*>(json.get());
			std::cout << "Received a bool: " << std::boolalpha << (bool) (*jsonBool) << '\n';
		}
		break;
	case json::JsonType::String:
		{
			json::JsonString* jsonStr = dynamic_cast<json::JsonString*>(json.get());
			std::cout << "Received a string: " << jsonStr->str() << '\n';
		}
		break;
	case json::JsonType::Number:
		{
			json::JsonNumber* jsonNum = dynamic_cast<json::JsonNumber*>(json.get());
			std::cout << "Received a number: " << jsonNum->str() << '\n';
		}
		break;
	case json::JsonType::Array:
		{
			json::JsonArray* arr = dynamic_cast<json::JsonArray*>(json.get());
			std::cout << "Received an array\n";
			for (auto& element : arr->elements) {
				// element: std::unique_ptr<json::JsonElement>
				(void)(element);
			}
		}
		break;
	case json::JsonType::Object:
		{
			json::JsonObject* obj = dynamic_cast<json::JsonObject*>(json.get());
			std::cout << "Received an object\n";
			for (auto& [key, element] : *obj) {
				// key: str, element: std::unique_ptr<json::JsonElement>
				(void)(key); (void)(element);
			}
		}
		break;
	default:
		std::unreachable();
	}

	// Writing
	json->write(out);
}

int main() {
	std::cout << "Your input:\n";
	std::string input;
	std::getline(std::cin, input);

	std::stringstream stream(input);
	json::JsonInput in(&stream);

	json::JsonObject obj;
	bool res = obj.read(in);

	std::cout << "-- Parsing result --\n";
	std::cout << "Success: " << std::boolalpha << res << '\n';
	std::cout << "-- Writing result --\n";
	std::cout << "Result: ";
	obj.write(std::cout);
	std::cout << "\nSuccess: " << std::boolalpha << res << '\n';

	return 0;
}