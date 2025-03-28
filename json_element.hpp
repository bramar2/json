#pragma once
#include "input.hpp"

#include <ostream>

namespace json {
	class JsonElement {
	public:
		virtual ~JsonElement() = default;
		virtual bool read(JsonInput& in) = 0;
		virtual void write(std::ostream& out) const = 0;
	};
}