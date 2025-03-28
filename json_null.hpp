#pragma once

#include "json_element.hpp"

namespace json {
	class JsonNull : public JsonElement {
	public:
		bool read(JsonInput& in);
		void write(std::ostream& out) const;
	};
}