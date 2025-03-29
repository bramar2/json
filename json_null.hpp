#pragma once

#include "json_element.hpp"

namespace json {
	class JsonNull : public JsonElement {
	public:
		JsonType type() const override;
		bool read(JsonInput& in) override;
		void write(std::ostream& out) const override;
		std::unique_ptr<JsonElement> clone() const override;
	};
}