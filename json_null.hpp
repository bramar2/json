#pragma once

#include "json_element.hpp"

namespace json {
	class JsonNull : public JsonElement {
	public:
		bool read(JsonInput& in);
		bool write(std::ostream& out) const;
	};
}