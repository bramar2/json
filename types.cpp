#include "types.hpp"

namespace json {

	bool write_escaped(std::ostream& out, const std::string& value) {
		for (char c : value) {
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
			case '"':
				out << '\\';
			default:
				if (c < ' ') {
					return false;
				}
				out << c;
			}
		}
		return true;
	}

	bool JsonNumber::write(std::ostream& out) const {
		out << this->value;
		return true;
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

		bool dot = false, exp = false, numAfterExp = false, valid = true, end = false;
		while ( valid && !end && (ch = in.next()) != '\0') {
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
					valid = false;
				}
				break;
			case 'e':
			case 'E':
				if (!exp) {
					exp = true;
					newValue.push_back('e');
					if (char peek = in.peek(); peek == '-' || peek == '+') {
						newValue.push_back(peek);
						in.ptr += 1;
					}
				} else {
					valid = false;
				}
				break;
			default:
				in.ptr -= 1;
				end = true;
				break;
			}
		}
		valid &= !exp || numAfterExp;
		if (valid) {
			this->value = std::move(newValue);
			return true;
		} else {
			return false;
		}
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


	bool JsonString::write(std::ostream& out) const {
		out << '"';
		if (!write_escaped(out, this->value)) return false;
		out << '"';
		return true;
	}

	bool JsonArray::write(std::ostream& out) const {
		out << '[';
		const size_t sz = this->elements.size();
		if (sz > 0) {
			this->elements[0]->write(out);

			for (size_t i = 1; i < sz; i++) {
				out << ',';
				if (!this->elements[i]->write(out)) return false;
			}
		}
		out << ']';
		return true;
	}
	bool JsonObject::write(std::ostream& out) const {
		out << '{';
		if (!this->members.empty()) {
			auto it = this->members.begin(), end = this->members.end();

			out << '"';
			if (!write_escaped(out, it->first)) return false;
			out << "\":";
			if (!it->second->write(out)) return false;

			for (++it; it != end; ++it) {
				out << ",\"";
				if (!write_escaped(out, it->first)) return false;
				out << "\":";
				if (!it->second->write(out)) return false;
			}
		}
		out << '}';
		return true;
	}

	bool JsonString::read(JsonInput& in) {
		return false;
	}
	bool JsonArray::read(JsonInput& in) {
		return false;
	}
	bool JsonObject::read(JsonInput& in) {
		return false;
	}
}