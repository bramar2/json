#include "types.hpp"
#include <cmath>

namespace json {
	JsonNumber::JsonNumber(long double c_value) : value(c_value) {}
	JsonString::JsonString(std::string_view c_value) : value(c_value) {}

	bool write_escaped(std::ostream& out, const std::string& value) {
		static constexpr std::array<std::string, 128> mapping = []() {
			std::array<std::string, 128> result {};
			for (int i = 32; i <= 126; i++) {
				result[i] = std::string(1, (char) i);
			}
			result['\\'] = "\\\\";
			result['"'] = "\\\"";
			result['\b'] = "\\b";
			result['\t'] = "\\t";
			result['\n'] = "\\n";
			result['\f'] = "\\f";
			result['\r'] = "\\r";

			return result;
		}();
		for (char c : value) {
			if (mapping[c].empty()) {
				return false;
			}
			out << mapping[c];
		}
		return true;
	}

	bool JsonNumber::write(std::ostream& out) const {
		// todo
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

	bool JsonNumber::read(JsonInput& in) {
		this->value = 0;
		this->exp = 0;
		bool sign = false, scientific = false;

		char tmp = in.next();
		if (tmp == '-') {
			sign = true;
			tmp = in.next();
		}
		if (std::isdigit(tmp)) {
			this->value = (tmp - '0');
		} else {
			return false;
		}
		// if zero, next has to be . or e or ending, otherwise it is an extraneous zero (not allowed).
		if (tmp == '0' && std::isdigit(in.peek())) {
			return false;
		}

		bool going = false;
		while (going && (tmp = in.next())) {
			switch (tmp) {
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				this->value = 10 * this->value + (tmp - '0');
				break;
			case 'e':
			case 'E':
				scientific = true;
				going = false;
				break;
			case '.':
				this->decimal = true;
				going = false;
				break;
			default:
				in.ptr -= 1;
				going = false;
				break;
			}
		}

		if (this->decimal) {
			if (!std::isdigit(in.peek())) {
				return false;
			}
			going = true;
			long double multiplier = 0.1;
			while (going && (tmp == in.next())) {
				switch (tmp) {
				case '0': case '1': case '2': case '3': case '4':
				case '5': case '6': case '7': case '8': case '9':
					this->value += multiplier * (tmp - '0');
					multiplier /= 10;
					break;
				case 'e':
				case 'E':
					scientific = true;
					going = false;
					break;
				default:
					in.ptr -= 1;
					going = false;
					break;
				}
			}
		}
		if (scientific) {
			char peek = in.peek();
			bool exp_sign = (peek == '-');
			size_t len = 0;
			if (peek == '-' || peek == '+') {
				in.ptr += 1;
			}
			going = true;
			while (going && (tmp == in.next())) {
				switch (tmp) {
				case '0': case '1': case '2': case '3': case '4':
				case '5': case '6': case '7': case '8': case '9':
					if(++len == 4) {
						return false;
					}
					this->exp = 10 * this->exp + (tmp - '0');
					break;
				default:
					in.ptr -= 1;
					going = false;
					break;
				}
			}
			if (len == 0) {
				return false;
			}
			if (exp_sign) {
				this->exp *= -1;
			}
			this->value *= std::powl(10, this->exp);
		}

		return true;
	}
}