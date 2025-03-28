#include "json_object.hpp"
#include "json_parse.hpp"
#include "json_string.hpp"

namespace json {
	bool JsonObject::write(std::ostream& out) const {
		out << '{';
		if (!this->members.empty()) {
			auto it = this->members.begin(), end = this->members.end();

			out << '"';
			if (!JsonString::write_escaped(out, it->first)) return false;
			out << "\":";
			if (!it->second->write(out)) return false;

			for (++it; it != end; ++it) {
				out << ",\"";
				if (!JsonString::write_escaped(out, it->first)) return false;
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
		char ch = in.next_iw();

		if (ch == '}') {
			this->members.clear();
			return true;
		}


		in.ptr -= 1;
		std::unordered_map<std::string, std::unique_ptr<JsonElement>> new_members;
		JsonString key;
		while (true) {
			ch = in.next_iw();
			if (ch != '"') {
				return false;
			}
			in.ptr -= 1;
			key.read(in);

			if (new_members.count(key.value)) {
				return false;
			}

			ch = in.next_iw();
			if (ch != ':') {
				return false;
			}

			std::unique_ptr<JsonElement> value = json::parse(in);
			if (!value) {
				return false;
			}

			new_members.emplace(std::move(key.value), std::move(value));

			ch = in.next_iw();
			if (ch != ',') {
				break;
			}
		}

		if (ch != '}') {
			return false;
		}

		this->members = std::move(new_members);
		return true;
	}
}