#pragma once

#include "json_null.hpp"

namespace json {
	bool JsonNull::write(std::ostream& out) const {
		out << "null";
		return true;
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