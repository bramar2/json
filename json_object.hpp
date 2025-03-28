#pragma once

#include "json_element.hpp"

#include <memory>
#include <unordered_map>

namespace json {	
	class JsonObject : public JsonElement {
	public:
		std::unordered_map<std::string, std::unique_ptr<JsonElement>> members;

		bool read(JsonInput& in) override;
		bool write(std::ostream& out) const override;
	};
}