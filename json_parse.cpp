#include "json_parse.hpp"
#include "json_types.hpp"

namespace json {
	std::unique_ptr<JsonElement> parse(JsonInput& in) {
		switch (in.next_iw()) {
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			{
				in.ptr -= 1;
				std::unique_ptr<JsonElement> ptr = std::make_unique<JsonNumber>();
				if (!ptr->read(in)) return nullptr;
				return ptr;
			}
		case '"':
			{
				in.ptr -= 1;
				std::unique_ptr<JsonElement> ptr = std::make_unique<JsonString>();
				if (!ptr->read(in)) return nullptr;
				return ptr;
			}
		case '[':
			{
				in.ptr -= 1;
				std::unique_ptr<JsonElement> ptr = std::make_unique<JsonArray>();
				if (!ptr->read(in)) return nullptr;
				return ptr;
			}
		case '{':
			{
				in.ptr -= 1;
				std::unique_ptr<JsonElement> ptr = std::make_unique<JsonObject>();
				if (!ptr->read(in)) return nullptr;
				return ptr;
			}
		case 'n':
			{
				in.ptr -= 1;
				std::unique_ptr<JsonElement> ptr = std::make_unique<JsonNull>();
				if (!ptr->read(in)) return nullptr;
				return ptr;
			}
		case 't':
		case 'f':
			{
				in.ptr -= 1;
				std::unique_ptr<JsonElement> ptr = std::make_unique<JsonBool>();
				if (!ptr->read(in)) return nullptr;
				return ptr;
			}
		default:
			return nullptr;
		}
	}
}