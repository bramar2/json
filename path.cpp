#include "path.hpp"
#include "json_array.hpp"
#include "json_object.hpp"

#include <iostream>
namespace json {

	JsonPath::JsonPath() {}
	JsonPath::JsonPath(const JsonPath& other) : path(other.path) {}
	JsonPath::JsonPath(JsonPath&& other) : path(std::move(other.path)) {};

	JsonElement* JsonPath::query(JsonElement* root) const {
		for (auto& next : this->path) {
			if (std::holds_alternative<std::string>(next)) {
				if (root->type() != JsonType::Object) {
					return nullptr;
				}

				JsonObject* obj = static_cast<JsonObject*>(root);
				const std::string& key = std::get<std::string>(next);
				std::cerr << "Visiting key: \"" << key << "\"\n";
				if (!obj->contains(key)) {
					return nullptr;
				}
				root = obj->at(key);
			} else {
				if (root->type() != JsonType::Array) {
					return nullptr;
				}

				JsonArray* arr = static_cast<JsonArray*>(root);
				const size_t idx = std::get<size_t>(next);
				if (idx >= arr->elements.size()) {
					return nullptr;
				}
				root = arr->elements[idx].get();
			}
		}
		return root;
	}

	std::optional<JsonPath> JsonPath::create(std::string_view query) {
		if (query.empty() || query[0] != '$') return std::nullopt;

		JsonPath res;

		const size_t sz = query.size();
		size_t ptr = 1;
		while (ptr < sz) {
			if (ptr + 1 >= sz) {
				return std::nullopt;
			}
			switch (query[ptr]) {
			case '.':
				{
					res.path.emplace_back("");
					std::string& key = std::get<std::string>(res.path.back());
					for (++ptr; ptr < sz; ++ptr) {
						if (std::isalnum(query[ptr])) {
							key.push_back(query[ptr]);
						} else {
							break;
						}
					}
					if (key.empty()) {
						return std::nullopt;
					}
				}
				break;
			case '[':
				{
					ptr += 1;
					if (query[ptr] == '"') {
						res.path.emplace_back("");
						std::string& key = std::get<std::string>(res.path.back());
						for(++ptr; ptr < sz; ++ptr) {
							if (query[ptr] == '\\') {
								if (++ptr == sz) {
									return std::nullopt;
								}
								switch (query[ptr]) {
								case '\\':
									key.push_back('\\');
									break;
								case '"':
									key.push_back('"');
									break;
								case '/':
									key.push_back('/');
									break;
								case 'n':
									key.push_back('\n');
									break;
								case 'b':
									key.push_back('\b');
									break;
								case 'r':
									key.push_back('\r');
									break;
								case 'f':
									key.push_back('\f');
									break;
								case 't':
									key.push_back('\t');
									break;
								default:
									return std::nullopt;
								}
							} else if (query[ptr] == '"') {
								break;
							} else {
								key.push_back(query[ptr]);
							}
						}
						ptr += 1;
						if (ptr >= sz || query[ptr] != ']') {
							return std::nullopt;
						}
						ptr += 1;
					} else if (query[ptr] == '0') {
						ptr += 1;
						if (ptr >= sz || query[ptr] != ']') {
							return std::nullopt;
						}
						ptr += 1;
						res.path.emplace_back(static_cast<size_t>(0));
					} else if (std::isdigit(query[ptr])) {
						std::string key(1, query[ptr]);
						for (++ptr; ptr < sz; ++ptr) {
							if (std::isdigit(query[ptr])) {
								key.push_back(query[ptr]);
							} else {
								break;
							}
						}
						if (key.size() > 10u || ptr >= sz || query[ptr] != ']') {
							return std::nullopt;
						}
						ptr += 1;

						res.path.emplace_back(static_cast<size_t>(stoull(key)));
					} else {
						return std::nullopt;
					}
				}
				break;
			}
		}
		return res;
	}
}