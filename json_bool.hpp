#pragma once

#include "json_element.hpp"

namespace json {
	class JsonBool : public JsonElement {
	public:
		bool value = false;

		bool read(JsonInput& in) override;
		bool write(std::ostream& out) const override;

		operator bool() const;
	};
}