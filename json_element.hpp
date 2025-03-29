#pragma once
#include "input.hpp"
#include "json_type.hpp"

#include <ostream>
#include <memory>

namespace json {
	class JsonElement {
	public:
		virtual ~JsonElement() = default;
		virtual JsonType type() const = 0;
		virtual bool read(JsonInput& in) = 0;
		virtual void write(std::ostream& out) const = 0;
		virtual std::unique_ptr<JsonElement> clone() const = 0;
	};
}