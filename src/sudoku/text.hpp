#pragma once

#include <array>
#include <inttypes.h>
#include <string>

namespace jkk::text{

	int utf8_encode(char* out, uint32_t code);


	struct UnicodeConv {
		std::string_view to_string_view(uint32_t code);
		std::array<char, 4> buff;
	};
	
}