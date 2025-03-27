#pragma once

#include "input.hpp"

#include <unordered_map>
#include <vector>
#include <memory>
#include <iostream>

namespace json {
	class JsonElement {
	public:
		virtual bool read(JsonInput& in);
		virtual bool write(std::ostream& out) const;
	};
	class JsonNumber : public JsonElement {
	public:
		long double value = 0;
		size_t decimal = 0;
		int exp = 0;

		bool read(JsonInput& in) override;
		bool write(std::ostream& out) const override;
	};
	class JsonString : public JsonElement {
	public:
		std::string value;

		bool read(JsonInput& in) override;
		bool write(std::ostream& out) const override;
	};
	class JsonArray : public JsonElement {
	public:
		std::vector<std::unique_ptr<JsonElement>> elements;

		bool read(JsonInput& in) override;
		bool write(std::ostream& out) const override;
	};
	class JsonObject : public JsonElement {
	public:
		std::unordered_map<std::string, std::unique_ptr<JsonElement>> members;

		bool read(JsonInput& in) override;
		bool write(std::ostream& out) const override;
	};
}