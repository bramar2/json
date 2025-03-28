#include "input.hpp"


namespace json {
	JsonInput::JsonInput(std::istream* c_in) : in(c_in) {}

	bool JsonInput::refill() {
		if (ptr != available) return true;
		in->read(buffer, BUFFER_SIZE);
		ptr = 0;
		available = in->gcount();
		return available != 0;
	}

	char JsonInput::next() {
		if (!refill()) return '\0';
		return buffer[ptr++];
	}

	char JsonInput::peek() {
		if (!refill()) return '\0';
		return buffer[ptr];
	}
	
	char JsonInput::next_iw() {
		char res;
		while (std::isspace( res = next() ));
		return res;
	}
}