#pragma once

#include <unordered_map>
#include <vector>
#include <memory>

namespace json {
	class JsonElement {};
	class JsonString : public JsonElement, public std::string {};
	class JsonInteger : public JsonElement {
	public:
		int64_t value;
		JsonInteger(int64_t c_value);
	};
	class JsonDecimal : public JsonElement {
	public:
		double value;
		JsonDecimal(double c_value);
	};
	class JsonArray : public JsonElement, public std::vector<std::unique_ptr<JsonElement>> {};
	class JsonObject : public JsonElement, public std::unordered_map<std::string, std::unique_ptr<JsonElement>> {};
}