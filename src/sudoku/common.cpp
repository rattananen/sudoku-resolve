#include "common.hpp"
#include "text.hpp"
#include <algorithm>
#include <iomanip>
#include <bitset>

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
				//os << n;
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

	std::ostream& operator<<(std::ostream& os, View& v)
	{
		for (auto it : v) {
			os << it << "\n";
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& os, Sub_view& v)
	{
		for (auto it:v) {
			os << static_cast<int>(it) << " ";
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& os, Marker m)
	{
		return os << std::bitset<9>(m.data);
	}

	Grid::value_type& Grid::operator[](size_t n)
	{
		return data[n];
	}

	Grid_span::value_type& Grid_span::operator[](size_t n)
	{
		return data[n];
	}

	
	constexpr size_t index_for(Grid::value_type n)
	{
		return static_cast<size_t>(n) - 1;
	}

	void Validator::validate(Marker& out, Sub_view sub) {
		
		out.data = -1u;
		lookup.data = 0;
		
		for (size_t pos = 0; pos != 9; ++pos) {
			auto v = sub[pos];
			if (v == 0) {
				out.set(pos, false);
				continue;
			}
			auto index = index_for(v);
			if (lookup.get(index)) {
				out.set(pos, false);
			}
			else {
				lookup.set(index, true);
			}
		}
	}
}