#pragma once

#include "json_element.hpp"

namespace json {
	class JsonBool : public JsonElement {
	public:
		bool value = false;
		JsonBool(bool c_value = false);
		
		JsonType type() const override;
		bool read(JsonInput& in) override;
		void write(std::ostream& out) const override;
		std::unique_ptr<JsonElement> clone() const override;

		operator bool() const;
	};
}