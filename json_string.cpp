#include "json_string.hpp"

namespace json {

	bool JsonString::valid(std::string_view str) {
		for (char c : str) {
			switch (c) {
			case '\b': case '\t': case '\n': case '\f': case '\r':
				break;
			default:
				if (0 < c && c < ' ') {
					return false;
				}
				break;
			}
		}
		return true;
	}

	void JsonString::write_escaped(std::ostream& out, std::string_view str) {
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
				out << c;
				break;
			}
		}
	}

	JsonString::JsonString() {}
	JsonString::JsonString(const JsonString& other) : value(other.value) {}
	JsonString::JsonString(JsonString&& other) : value(std::move(other.value)) {}

	JsonType JsonString::type() const {
		return JsonType::String;
	}

	void JsonString::write(std::ostream& out) const {
		out << '"';
		JsonString::write_escaped(out, this->value);
		out << '"';
	}
	bool JsonString::read(JsonInput& in) {

		if (in.next() != '"') {
			return false;
		}

		std::string newValue;
		char ch;
		bool end = false;
		while ( !end && (ch = in.next()) ) {
			if (0 < ch && ch < ' ') {
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
				case 'u':
					{
						char hexCode[5];
						hexCode[4] = '\0';
						for (int i = 0; i < 4; i++) {
							hexCode[i] = in.next();
							if (std::isdigit(hexCode[i]) || ('a' <= hexCode[i] && hexCode[i] <= 'f') || 
									('A' <= hexCode[i] && hexCode[i] <= 'F')) {
								continue;
							}
							return false;
						}
						int codePoint = std::strtol(hexCode, NULL, 16);
						if (codePoint <= 0x007F) {
							newValue.push_back(char(codePoint));
						} else if (codePoint <= 0x07FF) {
							newValue.push_back(char(
								0b11000000 | (codePoint >> 6)
								));
							newValue.push_back(char(
								0b10000000 | (codePoint & 0x3F)
								));
						} else if (0xD800 <= codePoint && codePoint <= 0xDBFF) { // surrogate pair
							if (in.next() != '\\' || in.next() != 'u') {
								return false;
							}
							for (int i = 0; i < 4; i++) {
								hexCode[i] = in.next();
								if (std::isdigit(hexCode[i]) || ('a' <= hexCode[i] && hexCode[i] <= 'f') || 
										('A' <= hexCode[i] && hexCode[i] <= 'F')) {
									continue;
								}
								return false;
							}
							int lowCodePoint = std::strtol(hexCode, NULL, 16);
							if (!(0xDC00 <= lowCodePoint && lowCodePoint <= 0xDFFF)) {
								return false;
							}
							int high = codePoint & 0x3FF, low = lowCodePoint & 0x3FF;
							codePoint = 0x10000 + ((high << 10) | low);
							newValue.push_back(char(
								0b11110000 | (codePoint >> 18)
								));
							newValue.push_back(char(
								0b10000000 | ((codePoint >> 12) & 0x3F)
								));
							newValue.push_back(char(
								0b10000000 | ((codePoint >> 6) & 0x3F)
								));
							newValue.push_back(char(
								0b10000000 | (codePoint & 0x3F)
								));
						} else {
							newValue.push_back(char(
								0b11100000 | (codePoint >> 12)
								));
							newValue.push_back(char(
								0b10000000 | ((codePoint >> 6) & 0x3F)
								));
							newValue.push_back(char(
								0b10000000 | (codePoint & 0x3F)
								));
						}
					}
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

	bool JsonString::set(std::string_view newValue) {
		if (valid(newValue)) {
			this->value = newValue;
			return true;
		} else {
			return false;
		}
	}
	bool JsonString::set(std::string newValue) {
		if (valid(newValue)) {
			this->value = std::move(newValue);
			return true;
		} else {
			return false;
		}
	}

	std::unique_ptr<JsonElement> JsonString::clone() const {
		std::unique_ptr<JsonString> ptr = std::make_unique<JsonString>();
		ptr->value = this->value;
		return ptr;
	}

	const std::string& JsonString::str() {
		return this->value;
	}
}