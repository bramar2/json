#pragma once

#include "json_element.hpp"

namespace json {
	class JsonBool : public JsonElement {
	public:
		bool value = false;

		JsonType type() const override;
		bool read(JsonInput& in) override;
		void write(std::ostream& out) const override;

		operator bool() const;
	};
}