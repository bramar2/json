#include "json_object.hpp"
#include "json_string.hpp"
#include "parse.hpp"

namespace json {

	JsonObject::JsonObject() {}
	JsonObject::JsonObject(const JsonObject& other) {
		this->members.reserve(other.members.size());
		for (const auto& [key, element] : other.members) {
			this->members.emplace(key, element->clone());
		}
	}
	JsonObject::JsonObject(JsonObject&& other) : members(std::move(other.members)) {}

	JsonType JsonObject::type() const {
		return JsonType::Object;
	}

	void JsonObject::write(std::ostream& out) const {
		out << '{';
		if (!this->members.empty()) {
			auto it = this->members.begin(), end = this->members.end();

			out << '"';
			JsonString::write_escaped(out, it->first);
			out << "\":";
			it->second->write(out);

			for (++it; it != end; ++it) {
				out << ",\"";
				JsonString::write_escaped(out, it->first);
				out << "\":";
				it->second->write(out);
			}
		}
		out << '}';
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

			if (new_members.count(key.str())) {
				return false;
			}

			ch = in.next_iw();
			if (ch != ':') {
				return false;
			}

			std::unique_ptr<JsonElement> value = json::parse(in, true);
			if (!value) {
				return false;
			}
			new_members.emplace(key.str(), std::move(value));

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

	std::unique_ptr<JsonElement> JsonObject::clone() const {
		std::unique_ptr<JsonObject> res = std::make_unique<JsonObject>();
		res->members.reserve(this->members.size());
		for (const auto& [key, element] : this->members) {
			res->members.emplace(key, element->clone());
		}
		return res;
	}


	JsonElement* JsonObject::at(const std::string& key) const {
		return this->members.at(key).get();
	}

	void JsonObject::clear() {
		this->members.clear();
	}

	bool JsonObject::contains(const std::string& key) const {
		return this->members.count(key);
	}

	bool JsonObject::erase(const std::string& key) {
		return static_cast<bool>(this->members.erase(key));
	}

	bool JsonObject::set(const std::string& key, std::unique_ptr<JsonElement> value) {
		if (auto it = this->members.find(key); it != this->members.end()) {
			it->second = std::move(value);
		} else if (JsonString::valid(key)) {
			this->members.emplace(key, std::move(value));
		} else {
			return false;
		}
		return true;
	}

	std::unordered_map<std::string, std::unique_ptr<JsonElement>>::iterator JsonObject::begin() {
		return this->members.begin();
	}
	
	std::unordered_map<std::string, std::unique_ptr<JsonElement>>::iterator JsonObject::end() {
		return this->members.end();
	}
}