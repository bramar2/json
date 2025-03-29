#pragma once
#include "json_element.hpp"

namespace json {
	class JsonString : public JsonElement {
	private:
		std::string value;
	public:
		JsonString();
		JsonString(const JsonString& other);
		JsonString(JsonString&& other);

		JsonType type() const override;
		bool read(JsonInput& in) override;
		void write(std::ostream& out) const override;
		std::unique_ptr<JsonElement> clone() const override;

		bool set(std::string_view newValue);
		bool set(std::string newValue);
		const std::string& str();

		static void write_escaped(std::ostream& out, std::string_view str);
		static bool valid(std::string_view str);
	};	
}