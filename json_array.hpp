#pragma once
#include "json_element.hpp"

#include <vector>

namespace json {
	class JsonArray : public JsonElement {
	public:
		std::vector<std::unique_ptr<JsonElement>> elements;
		JsonArray();
		JsonArray(const JsonArray& other);
		JsonArray(JsonArray&& other);

		JsonType type() const override;
		bool read(JsonInput& in) override;
		void write(std::ostream& out) const override;
		std::unique_ptr<JsonElement> clone() const override;
	};
}