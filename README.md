## bramar2/json: A C++ Json library
Library is inspired by Gson (Google's Json library in Java) as seen by its code. CMake and C++23 standard. Does not support unicode.

## Building
At root, run the following:
- `cmake -S . -B build/`
- `cmake --build build/ --config [Debug/Release]`

## Usage
```cpp
#include "json/json.hpp"
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
			json::JsonBool* jsonBool = static_cast<json::JsonBool*>(json.get());
			std::cout << "Received a bool: " << std::boolalpha << (bool) (*jsonBool) << '\n';
		}
		break;
	case json::JsonType::String:
		{
			json::JsonString* jsonStr = static_cast<json::JsonString*>(json.get());
			std::cout << "Received a string: " << jsonStr->str() << '\n';
		}
		break;
	case json::JsonType::Number:
		{
			json::JsonNumber* jsonNum = static_cast<json::JsonNumber*>(json.get());
			std::cout << "Received a number: " << jsonNum->str() << '\n';
		}
		break;
	case json::JsonType::Array:
		{
			json::JsonArray* arr = static_cast<json::JsonArray*>(json.get());
			std::cout << "Received an array\n";
			for (auto& element : arr->elements) {
				// element: std::unique_ptr<json::JsonElement>
				(void)(element);
			}
		}
		break;
	case json::JsonType::Object:
		{
			json::JsonObject* obj = static_cast<json::JsonObject*>(json.get());
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

	// Pathing
	static const json::JsonPath CONST_JSON_PATH = json::JsonPath::create("$.myPath")
			.or_else([]() -> std::optional<json::JsonPath> { throw std::runtime_error("invalid json path in CONST_JSON_PATH"); })
			.value();
	std::optional<json::JsonPath> pathOpt = json::JsonPath::create("$.test[0].mykey[\"key with spaces\"]");
	if (!pathOpt) {
		throw "invalid path";
	}
	json::JsonPath path = std::move(pathOpt.value());
	json::JsonElement* dest = path.query(json.get());

	if (!dest) {
		throw "json doesnt match path";
	} else {
		std::cout << "Received destination from pathing: ";
		dest->write(std::cout);
		std::cout << '\n';
	}

	// Writing
	json->write(out);
}
```