#pragma once

#include "input.hpp"

#include <unordered_map>
#include <vector>
#include <memory>
#include <iostream>

namespace json {
	class JsonElement {
	public:
		virtual bool read(JsonInput& in) = 0;
		virtual bool write(std::ostream& out) const = 0;
	};
	class JsonNumber : public JsonElement {
	private:
		std::string value = "0";
	public:
		bool read(JsonInput& in) override;
		bool write(std::ostream& out) const override;

		operator int() const;
		operator long() const;
		operator long long() const;
		operator unsigned long() const;
		operator unsigned long long() const;

		operator float() const;
		operator double() const;
		operator long double() const;
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