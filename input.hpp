#pragma once

#include <istream>

namespace json {
	class JsonInput {
	public:
		static constexpr size_t BUFFER_SIZE = 1024;
		std::istream* in;
		char buffer[BUFFER_SIZE];
		size_t ptr = 0, available = 0;

		JsonInput(std::istream* c_in);
		char read();
		char peek();
	};
}