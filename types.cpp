#include "types.hpp"

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
}