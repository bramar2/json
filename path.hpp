#pragma once

#include "json_element.hpp"

#include <vector>

namespace json {
	class JsonPath {
	private:
		std::vector<std::variant<std::string, size_t>> path;
		JsonPath();
	public:
		JsonPath(const JsonPath& other);
		JsonPath(JsonPath&& other);
		JsonElement* query(JsonElement* root) const;
		static std::optional<JsonPath> create(std::string_view path);
	};
}