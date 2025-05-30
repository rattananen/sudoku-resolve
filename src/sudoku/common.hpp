#pragma once

#include <inttypes.h>
#include <array>
#include <string>
#include <iostream>


namespace jkk::sudoku{

	void create_table_line(std::string& out, size_t cell_w, size_t col, size_t sub, uint32_t beg = L'┏', uint32_t line = L'━', uint32_t end = L'┓', uint32_t sep = L'┯', uint32_t sep_sub = L'┳');

	//store in row-wise
	struct Grid {
		std::array<int32_t, 81> data; 
	};

	struct Grid_row_view {
		Grid_row_view(Grid& gd, size_t pos);

		int32_t& operator[](size_t n);
	    
		size_t m_pos;
		Grid& m_gd;
	};

	std::ostream& operator<<(std::ostream& os, const Grid& t);
}