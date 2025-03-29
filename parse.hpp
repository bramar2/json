#pragma once

#include "input.hpp"
#include "json_element.hpp"
#include <memory>

namespace json {
	std::unique_ptr<JsonElement> parse(JsonInput& in, bool _partial = false);
}