#include "input.hpp"


namespace json {
	JsonInput::JsonInput(std::istream* c_in) : in(c_in) {}
	char JsonInput::read() {
		if (ptr == available) {
			in->read(buffer, BUFFER_SIZE);
			ptr = 0;
			available = in->gcount();
			if (available == 0) return '\0';
		}
		return buffer[ptr++];
	}
	char JsonInput::peek() {
		if (ptr == available) {
			in->read(buffer, BUFFER_SIZE);
			ptr = 0;
			available = in->gcount();
			if (available == 0) return '\0';
		}
		return buffer[ptr];
	}
}