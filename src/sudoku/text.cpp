#include "text.hpp"


namespace jkk::text {

	int utf8_encode(char* out, uint32_t code)
	{
		if (code <= 0x7F) {
			out[0] = static_cast<char>(code);
			return 1;
		}
		else if (code <= 0x07FF) {
			out[0] = static_cast<char>(((code >> 6) & 0x1F) | 0xC0);
			out[1] = static_cast<char>(((code >> 0) & 0x3F) | 0x80);
			return 2;
		}
		else if (code <= 0xFFFF) {
			out[0] = static_cast<char>(((code >> 12) & 0x0F) | 0xE0);
			out[1] = static_cast<char>(((code >> 6) & 0x3F) | 0x80);
			out[2] = static_cast<char>(((code >> 0) & 0x3F) | 0x80);
			return 3;
		}
		else if (code <= 0x10FFFF) {
			out[0] = static_cast<char>(((code >> 18) & 0x07) | 0xF0);
			out[1] = static_cast<char>(((code >> 12) & 0x3F) | 0x80);
			out[2] = static_cast<char>(((code >> 6) & 0x3F) | 0x80);
			out[3] = static_cast<char>(((code >> 0) & 0x3F) | 0x80);
			return 4;
		}
		return 0; //error encoding
	}


	
	std::string_view UnicodeConv::to_string_view(uint32_t code) {
		int len = utf8_encode(buff.data(), code);
		if (len == 0) {
			return {};
		}
		return std::string_view(buff.data(), len);
	}
	

}