#include "json_array.hpp"
#include "parse.hpp"

namespace json {
	void JsonArray::write(std::ostream& out) const {
		out << '[';
		const size_t sz = this->elements.size();
		if (sz > 0) {
			this->elements[0]->write(out);

			for (size_t i = 1; i < sz; i++) {
				out << ',';
				this->elements[i]->write(out);
			}
		}
		out << ']';
	}

	bool JsonArray::read(JsonInput& in) {
		if (in.next_iw() != '[') {
			return false;
		}
		char ch = in.next_iw();

		if (ch == ']') {
			this->elements.clear();
			return true;
		}


		in.ptr -= 1;
		std::vector<std::unique_ptr<JsonElement>> new_elements;
		while (true) {
			ch = in.next_iw();
			in.ptr -= 1;

			std::unique_ptr<JsonElement> element = json::parse(in, true);
			if (!element) {
				return false;
			}
			new_elements.push_back(std::move(element));


			ch = in.next_iw();
			if (ch != ',') break;
		}
		if (ch != ']') {
			return false;
		}

		this->elements = std::move(new_elements);
		return true;
	}
}