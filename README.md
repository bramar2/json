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

std::istream in;
std::ostream out;


// Reading
json::JsonInput j_in(&in);
std::unique_ptr<json::JsonElement> json = json::parse(j_in);
if (!json) {
	throw "malformed json";
}
if (auto* jsonNull = dynamic_cast<json::JsonNull*>(json.get())) {
	std::cout << "Received null\n";
}else if (auto* jsonBool = dynamic_cast<json::JsonBool*>(json.get())) {
	std::cout << "Received a bool: " << std::boolalpha << (bool)jsonBool << '\n';
}else if (auto* str = dynamic_cast<json::JsonString*>(json.get())) {
	std::cout << "Received a string: " << str->str() << '\n';
} else if (auto* num = dynamic_cast<json::JsonNumber*>(json.get())) {
	std::cout << "Received a number: " << num->str() << '\n';
} else if (auto* arr = dynamic_cast<json::JsonArray*>(json.get())) {
	std::cout << "Received an array\n";
	for (auto& elements : arr->elements) {
		// do stuff
	}
} else if (auto* obj = dynamic_cast<json::JsonObject*>(json.get())) {
	std::cout << "Received an object\n";
	for (auto& [key, element] : *obj) {
		// do stuff
	}
} else {
	std::unreachable();
}

// Writing
json->write(out);

```