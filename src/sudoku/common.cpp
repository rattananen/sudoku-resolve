#include "common.hpp"
#include "text.hpp"
#include <algorithm>
#include <iomanip>

namespace jkk::sudoku {

	void create_table_line(std::string& out, size_t cell_w, size_t col, size_t sub,
		uint32_t beg,
		uint32_t line ,
		uint32_t end ,
		uint32_t sep,
		uint32_t sep_sub ) {


		text::UnicodeConv cv{};

		auto line_pt = cv.to_string_view(line);

		std::string cell_str;
		cell_str.reserve(cell_w * line_pt.size());
		for (int i = 0; i < cell_w; ++i) {
			cell_str += line_pt;
		}

		out.reserve(300);
		out += cv.to_string_view(beg);

		std::string sep_pt{ cv.to_string_view(sep) };
		std::string sep_sub_pt{ cv.to_string_view(sep_sub) };

		for (size_t i = 0; i < col; ++i) {
			if (i != 0) {
				out += (i % sub == 0) ? sep_sub_pt : sep_pt;
			}
			out += cell_str;
		}
		out += cv.to_string_view(end);
	}

	std::ostream& operator<<(std::ostream& os, const Grid& t) {
		constexpr size_t tsize = 9;
		constexpr size_t wsize = 3;
		constexpr size_t end = 81;
		std::string line;
		create_table_line(line, wsize, tsize, wsize);
		text::UnicodeConv cv{};
		
		os << line << "\n";

		line.clear();

		create_table_line(line, wsize, tsize, wsize, L'┠', L'─', L'┨', L'┼', L'╂');
	
		size_t n = 0;

		std::string sep{ cv.to_string_view(L'│') };
		std::string sep_thick{ cv.to_string_view(L'┃') };
		std::string sub_line;
		create_table_line(sub_line, wsize, tsize, wsize, L'┣', L'━', L'┫', L'┿', L'╋');

		for (size_t n = 0; n != end; ++n) {
			auto pos = n + 1;

			if (pos % 9 == 1) {
				os << sep_thick;
			}

			os << std::setw(wsize);
			if (t.data[n] == 0) {
				os << ' ';
			}
			else {
				os << static_cast<int>(t.data[n]);
			}

			if (pos % 9 != 0) {
				os << ((pos % 3 == 0) ? sep_thick : sep);
			}
			else if(pos != end){
				os << sep_thick << "\n" << ((pos % 27 == 0)? sub_line :line) << "\n";
			}

		}
		line.clear();

		create_table_line(line, wsize, tsize, wsize, L'┗', L'━', L'┛', L'┷', L'┻');

		os << sep_thick << "\n" << line << "\n";
		return os;
	}


	Grid::value_type& Grid::operator[](size_t n)
	{
		return data[n];
	}

	const Grid::value_type& Grid::operator[](size_t n) const
	{
		return data[n];
	}

	const Grid::value_type& Grid::operator()(size_t row, size_t col) const
	{
		return data[row * 9 + col];
	}

	Grid::value_type& Grid::as_sub(size_t sub_n, size_t n)
	{
		 return data[(27 * (sub_n / 3) + 3 * (sub_n % 3)) + (9 * (n / 3) + (n % 3))];
	}

	Grid::value_type& Grid::operator()(size_t row, size_t col)
	{
		return data[row * 9 + col];
	}

	bool Grid::is_full()
	{
		for (auto v:data) {
			if (v == 0) {
				return false;
			}
		}
		return true;
	}

	bool is_ok(const Grid& in)
	{
		for (size_t row = 0; row < 9; ++row) {
			for (size_t col = 0; col < 9; ++col) {
				if (in(row, col) == 0) {
					continue;
				}
				if (!is_ok(in, row, col)) {
					return false;
				}
			}
		}
		return true;
	}

	bool is_ok(const Grid& in, size_t row, size_t col) {
		auto num = in(row, col);
		for (size_t n = 0; n < 9; ++n) {
			if (n != col  && in(row, n) == num) {
				return false;
			}
		}

		for (size_t n = 0; n < 9; ++n) {
			if (n != row && in(n, col) == num) {
				return false;
			}
		}

		size_t start_row = row - row % 3, start_col = col - col % 3;
		for (size_t n = 0; n < 3; ++n)
			for (size_t k = 0; k < 3; ++k) {
				auto x = n + start_row;
				auto y = k + start_col;
				if (!(x == row && y == col) && in(x, y) == num)
					return false;
			}
		return true;
	}

}