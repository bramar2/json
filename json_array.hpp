#pragma once
#include "json_element.hpp"

#include <vector>
#include <memory>

namespace json {
	class JsonArray : public JsonElement {
	public:
		std::vector<std::unique_ptr<JsonElement>> elements;

		JsonType type() const override;
		bool read(JsonInput& in) override;
		void write(std::ostream& out) const override;
	};
}