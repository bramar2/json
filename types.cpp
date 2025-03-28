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
	std::unique_ptr<JsonElement> parse_unknown(JsonInput& in) {
		switch (in.next_iw()) {
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			{
				in.ptr -= 1;
				std::unique_ptr<JsonElement> ptr = std::make_unique<JsonNumber>();
				if (!ptr->read(in)) return nullptr;
				return ptr;
			}
		case '"':
			{
				in.ptr -= 1;
				std::unique_ptr<JsonElement> ptr = std::make_unique<JsonString>();
				if (!ptr->read(in)) return nullptr;
				return ptr;
			}
		case '[':
			{
				in.ptr -= 1;
				std::unique_ptr<JsonElement> ptr = std::make_unique<JsonArray>();
				if (!ptr->read(in)) return nullptr;
				return ptr;
			}
		case '{':
			{
				in.ptr -= 1;
				std::unique_ptr<JsonElement> ptr = std::make_unique<JsonObject>();
				if (!ptr->read(in)) return nullptr;
				return ptr;
			}
		default:
			return nullptr;
		}
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
	std::string JsonNumber::to_string() const {
		return this->value;
	}


	bool JsonString::write(std::ostream& out) const {
		out << '"';
		if (!write_escaped(out, this->value)) return false;
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
				switch (ch = in.next()) {
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

	bool JsonArray::read(JsonInput& in) {
		if (in.next_iw() != '[') {
			return false;
		}
		std::vector<std::unique_ptr<JsonElement>> new_elements;

		char ch = in.next_iw();
		bool end = false;
		if (ch == ']') {
			end = true;
		} else {
			in.ptr -= 1;
		}
		while (!end) {
			ch = in.next_iw();
			in.ptr -= 1;

			std::unique_ptr<JsonElement> element = parse_unknown(in);
			if (!element) {
				return false;
			}
			new_elements.push_back(std::move(element));


			ch = in.next_iw();
			end = (ch != ',');
		}
		if (ch != ']') {
			return false;
		}

		this->elements = std::move(new_elements);
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
	
	bool JsonObject::read(JsonInput& in) {
		if (in.next_iw() != '{') {
			return false;
		}
		std::unordered_map<std::string, std::unique_ptr<JsonElement>> new_members;

		char ch = in.next_iw();
		bool end = false;
		if (ch == '}') {
			end = true;
		} else {
			in.ptr -= 1;
		}
		JsonString key;
		while (!end) {
			ch = in.next_iw();
			if (ch != '"') {
				return false;
			}
			in.ptr -= 1;
			key.read(in);

			ch = in.next_iw();
			if (ch != ':') {
				return false;
			}

			std::unique_ptr<JsonElement> value = parse_unknown(in);
			if (!value) {
				return false;
			}

			new_members.emplace(std::move(key.value), std::move(value));

			ch = in.next_iw();
			end = (ch != ',');
		}

		if (ch != '}') {
			return false;
		}

		this->members = std::move(new_members);
		return true;
	}
}