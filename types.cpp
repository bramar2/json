#include "types.hpp"

namespace json {
	JsonInteger::JsonInteger(int64_t c_value) : value(c_value) {}
	JsonDecimal::JsonDecimal(double c_value) : value(c_value) {}
}