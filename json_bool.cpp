#pragma once

#include "json_bool.hpp"

namespace json {
	bool JsonBool::write(std::ostream& out) const {
		out << (value ? "true" : "false");
		return true;
	}

	bool JsonBool::read(JsonInput& in) {
		static constexpr std::string_view true_repr = "rue";
		static constexpr std::string_view false_repr = "alse";
		char ch = in.next();
		switch (ch) {
		case 't':
			for (char c : true_repr) {
				if (in.next() != c) {
					return false;
				}
			}
			this->value = true;
			break;
		case 'f':
			for (char c : false_repr) {
				if (in.next() != c) {
					return false;
				}
			}
			this->value = false;
			break;
		default:
			return false;
		}
		return true;
	}
	JsonBool::operator bool() const {
		return this->value;
	}
}