#pragma once

#include "json_element.hpp"

namespace json {
	class JsonNumber : public JsonElement {
	private:
		std::string value = "0";
	public:
		bool read(JsonInput& in) override;
		bool write(std::ostream& out) const override;

		operator int() const;
		operator long() const;
		operator long long() const;
		operator unsigned long() const;
		operator unsigned long long() const;

		operator float() const;
		operator double() const;
		operator long double() const;

		std::string to_string() const;
	};
}