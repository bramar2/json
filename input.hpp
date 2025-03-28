#pragma once

#include <istream>

namespace json {
	class JsonInput {
	public:
		static constexpr size_t BUFFER_SIZE = 4096;
		std::istream* in;
		char buffer[BUFFER_SIZE];
		size_t ptr = 0, available = 0;
	private:
		bool refill();
	public:
		JsonInput(std::istream* c_in);
		char next();
		char peek();
		char next_iw(); // next ignore whitespace
	};
}