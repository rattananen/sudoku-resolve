#pragma once

#include <inttypes.h>
#include <array>
#include <string>
#include <iostream>


namespace jkk::sudoku {


	void create_table_line(std::string& out, size_t cell_w, size_t col, size_t sub, uint32_t beg = L'┏', uint32_t line = L'━', uint32_t end = L'┓', uint32_t sep = L'┯', uint32_t sep_sub = L'┳');


	template<typename T, typename VAL>
	struct Grid_elem_range_iter
	{
		using elem_t = T;
		using value_type = VAL;
		void operator++()
		{
			++pos;
		}
		value_type& operator*()
		{
			return m_elem[pos];
		}
		bool operator==(std::default_sentinel_t) const
		{
			return pos >= 9;
		}

		explicit Grid_elem_range_iter(elem_t& elem) :
			m_elem{ view }, pos{ 0 }
		{
		}

	private:
		size_t pos;
		elem_t& m_elem;
	};

	enum struct Grid_element_type {
		row, col, region
	};



	//store in row-wise
	struct Grid {
		struct Element {
			int32_t& operator[](size_t n);

			std::array<int32_t, 9> data;
		};

		int32_t& operator[](size_t n);

		std::array<int32_t, 81> data;
	};

	template<Grid_element_type T>
	struct Element_value_locator {
		constexpr size_t operator()(size_t el_pos, size_t val_pos);
	};

	template<>
	struct Element_value_locator<Grid_element_type::row> {
		constexpr size_t operator()(size_t el_pos, size_t val_pos) {
			return el_pos * 9 + val_pos;
		}
	};

	template<>
	struct Element_value_locator<Grid_element_type::col> {
		constexpr size_t operator()(size_t el_pos, size_t val_pos) {
			return val_pos  * 9 + el_pos;
		}
	};

	template<>
	struct Element_value_locator<Grid_element_type::region> {
		constexpr size_t operator()(size_t el_pos, size_t val_pos) {
			return el_pos * 9 + val_pos;
		}
	};

	

	
	struct Grid_validator {
		struct Result {
			operator bool() const;
			bool& operator[](size_t i);
			void fill(bool v);
			std::array<bool, 9> data;
		};

		void validate(Result& out, Grid::Element& elm);

		
		Result lookup;

		constexpr static size_t index_for(int32_t n);
	};


	std::ostream& operator<<(std::ostream& os, const Grid& t);

	


}