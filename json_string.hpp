#pragma once
#include "json_element.hpp"

namespace json {
	class JsonString : public JsonElement {
	public:
		std::string value;

		bool read(JsonInput& in) override;
		void write(std::ostream& out) const override;

		static void write_escaped(std::ostream& out, std::string_view str);
		static bool valid(std::string_view str);
	};	
}