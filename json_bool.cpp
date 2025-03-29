#include "json_bool.hpp"

namespace json {

	JsonBool::JsonBool(bool c_value) : value(c_value) {}

	JsonType JsonBool::type() const {
		return JsonType::Bool;
	}

	void JsonBool::write(std::ostream& out) const {
		out << (this->value ? "true" : "false");
	}

	bool JsonBool::read(JsonInput& in) {
		static constexpr std::string_view repr[] = {"alse", "rue"};
		char ch = in.next();
		if (ch != 't' && ch != 'f') {
			return false;
		}
		for (char c : repr[ch == 't']) {
			if (in.next() != c) {
				return false;
			}
		}
		this->value = (ch == 't');
		return true;
	}

	std::unique_ptr<JsonElement> JsonBool::clone() const {
		return std::make_unique<JsonBool>(this->value);
	}

	JsonBool::operator bool() const {
		return this->value;
	}
}