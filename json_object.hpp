#pragma once

#include "json_element.hpp"

#include <memory>
#include <unordered_map>

namespace json {	
	class JsonObject : public JsonElement {
	private:
		std::unordered_map<std::string, std::unique_ptr<JsonElement>> members;
	public:
		JsonObject();
		JsonObject(const JsonObject& other);
		JsonObject(JsonObject&& other);

		JsonType type() const override;
		bool read(JsonInput& in) override;
		void write(std::ostream& out) const override;
		std::unique_ptr<JsonElement> clone() const override;

		JsonElement* at(const std::string& key) const;
		void clear();
		bool contains(const std::string& key) const;
		bool erase(const std::string& key);
		bool set(const std::string& key, std::unique_ptr<JsonElement> value);

		std::unordered_map<std::string, std::unique_ptr<JsonElement>>::iterator begin();
		std::unordered_map<std::string, std::unique_ptr<JsonElement>>::iterator end();
	};
}