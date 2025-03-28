#include "json_null.hpp"

namespace json {
	void JsonNull::write(std::ostream& out) const {
		out << "null";
	}

	bool JsonNull::read(JsonInput& in) {
		static constexpr std::string_view representation = "null";
		for (char ch : representation) {
			if (in.next() != ch) {
				return false;
			}
		}
		return true;
	}
}