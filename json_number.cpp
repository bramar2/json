#include "json_number.hpp"

namespace json {

	JsonNumber::JsonNumber() : value("0") {}
	JsonNumber::JsonNumber(const JsonNumber& other) : value(other.value) {}
	JsonNumber::JsonNumber(JsonNumber&& other) : value(std::move(other.value)) {}

	JsonType JsonNumber::type() const {
		return JsonType::Number;
	}

	void JsonNumber::write(std::ostream& out) const {
		out << this->value;
	}

	bool JsonNumber::read(JsonInput& in) {
		std::string newValue;
		char ch = in.next();

		if (ch == '-') {
			newValue.push_back('-');
			ch = in.next();
		}
		if (!std::isdigit(ch) || (ch == '0' && std::isdigit(in.peek()))) {
			return false;
		}
		newValue.push_back(ch);

		bool dot = false, exp = false, numAfterExp = false, end = false;
		while ( !end && (ch = in.next()) != '\0') {
			switch (ch) {
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				newValue.push_back(ch);
				numAfterExp |= exp;
				break;
			case '.':
				if (!dot && !exp && std::isdigit(in.peek())) {
					dot = true;
					newValue.push_back(ch);
				} else {
					return false;
				}
				break;
			case 'e':
			case 'E':
				if (!exp) {
					exp = true;
					newValue.push_back(ch);
					if (char peek = in.peek(); peek == '-' || peek == '+') {
						newValue.push_back(peek);
						in.ptr += 1;
					}
				} else {
					return false;
				}
				break;
			default:
				in.ptr -= 1;
				end = true;
				break;
			}
		}
		if (exp && !numAfterExp) {
			return false;
		}
		this->value = std::move(newValue);
		return true;
	}

	std::unique_ptr<JsonElement> JsonNumber::clone() const {
		std::unique_ptr<JsonNumber> ptr = std::make_unique<JsonNumber>();
		ptr->value = this->value;
		return ptr;
	}

	JsonNumber::operator int() const {
		return std::stoi(this->value);
	}
	JsonNumber::operator long() const {
		return std::stol(this->value);
	}
	JsonNumber::operator long long() const {
		return std::stoll(this->value);
	}
	JsonNumber::operator unsigned long() const {
		return std::stoul(this->value);
	}
	JsonNumber::operator unsigned long long() const {
		return std::stoull(this->value);
	}
	JsonNumber::operator float() const {
		return std::stof(this->value);
	}
	JsonNumber::operator double() const {
		return std::stod(this->value);
	}
	JsonNumber::operator long double() const {
		return std::stold(this->value);
	}
	const std::string& JsonNumber::str() const {
		return this->value;
	}
}