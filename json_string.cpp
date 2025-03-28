#include "json_string.hpp"

namespace json {

	bool JsonString::write_escaped(std::ostream& out, std::string_view str) {
		for (char c : str) {
			switch (c) {
			case '\b':
				out << "\\\b";
				break;
			case '\t':
				out << "\\\t";
				break;
			case '\n':
				out << "\\\n";
				break;
			case '\f':
				out << "\\\f";
				break;
			case '\r':
				out << "\\\r";
				break;
			case '\\':
				out << "\\\\";
				break;
			case '"':
				out << "\\\"";
				break;
			default:
				if (c < ' ') {
					return false;
				}
				out << c;
			}
		}
		return true;
	}

	bool JsonString::write(std::ostream& out) const {
		out << '"';
		JsonString::write_escaped(out, this->value);
		out << '"';
		return true;
	}
	bool JsonString::read(JsonInput& in) {

		if (in.next() != '"') {
			return false;
		}

		std::string newValue;
		char ch;
		bool end = false;
		while ( !end && (ch = in.next()) ) {
			if (ch < ' ') {
				return false;
			}
			switch (ch) {
			case '\\':
				ch = in.next();
				switch (ch) {
				case 'b':
					newValue.push_back('\b');
					break;
				case 't':
					newValue.push_back('\t');
					break;
				case 'n':
					newValue.push_back('\n');
					break;
				case 'f':
					newValue.push_back('\f');
					break;
				case 'r':
					newValue.push_back('\r');
					break;
				case '\\':
					newValue.push_back('\\');
					break;
				case '/':
					newValue.push_back('/');
					break;
				case '"':
					newValue.push_back('"');
					break;
				default:
					return false;
				}
				break;
			case '"':
				end = true;
				break;
			default:
				newValue.push_back(ch);
				break;
			}
		}
		if (!end) {
			return false;
		}

		this->value = std::move(newValue);
		return true;
	}
}