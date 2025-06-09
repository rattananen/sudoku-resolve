#pragma once

#include <inttypes.h>
#include <array>
#include <string>
#include <iostream>

namespace jkk::sudoku {

	
	void create_table_line(std::string& out, size_t cell_w, size_t col, size_t sub, uint32_t beg = L'┏', uint32_t line = L'━', uint32_t end = L'┓', uint32_t sep = L'┯', uint32_t sep_sub = L'┳');


	//store in row-wise
	struct Grid {
		using value_type = uint16_t;

		value_type& operator[](size_t n);
		const value_type& operator[](size_t n) const;

		value_type& operator()(size_t row, size_t col);
		const value_type& operator()(size_t row, size_t col) const;

		value_type& as_sub(size_t sub_n, size_t n);

		bool is_full();

		bool is_bad();

		std::array<value_type, 81> data;
	};


	bool is_ok(const Grid& in);
	bool is_ok(const Grid& in, size_t row, size_t col);


	std::ostream& operator<<(std::ostream& os, const Grid& t);

	void draw_table(std::ostream& os, const Grid& t);


}