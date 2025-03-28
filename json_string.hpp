#pragma once
#include "json_element.hpp"

namespace json {
	class JsonString : public JsonElement {
	public:
		std::string value;

		bool read(JsonInput& in) override;
		bool write(std::ostream& out) const override;

		static bool write_escaped(std::ostream& out, std::string_view str);
	};	
}