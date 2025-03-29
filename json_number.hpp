#pragma once

#include "json_element.hpp"

namespace json {
	class JsonNumber : public JsonElement {
	private:
		std::string value;
	public:
		JsonNumber();
		JsonNumber(const JsonNumber& other);
		JsonNumber(JsonNumber&& other);

		JsonType type() const override;
		bool read(JsonInput& in) override;
		void write(std::ostream& out) const override;
		std::unique_ptr<JsonElement> clone() const override;

		operator int() const;
		operator long() const;
		operator long long() const;
		operator unsigned long() const;
		operator unsigned long long() const;

		operator float() const;
		operator double() const;
		operator long double() const;

		const std::string& str() const;
	};
}