#pragma once

#include <inttypes.h>
#include <array>
#include <string>
#include <iostream>
#include <functional>

namespace jkk::sudoku {

	
	void create_table_line(std::string& out, size_t cell_w, size_t col, size_t sub, uint32_t beg = L'┏', uint32_t line = L'━', uint32_t end = L'┓', uint32_t sep = L'┯', uint32_t sep_sub = L'┳');


	template<typename T, typename VAL, typename REF = VAL&,  size_t SIZE = 9>
	struct Elem_iter
	{
		using elem_type = T;
		using value_type = VAL;
		using reference = REF;
		void operator++()
		{
			++pos;
		}
		reference operator*()
		{
			return m_elem[pos];
		}
		bool operator==(std::default_sentinel_t) const
		{
			return pos >= SIZE;
		}

		explicit Elem_iter(elem_type& elem) :
			m_elem{ elem }, pos{ 0 }
		{
		}

	private:
		size_t pos;
		elem_type& m_elem;
	};

	

	//store in row-wise
	struct Grid {
		using value_type = uint16_t;
		value_type& operator[](size_t n);
		bool is_full();

		std::array<value_type, 81> data;
	};

	enum struct View_type {
		row, col, region, cell
	};

	template<View_type T>
	struct View_value_locator {
		constexpr size_t operator()(size_t el_pos, size_t val_pos);
	};

	template<>
	struct View_value_locator<View_type::row> {
		constexpr size_t operator()(size_t el_pos, size_t val_pos) {
			return el_pos * 9 + val_pos;
		}
	};

	template<>
	struct View_value_locator<View_type::col> {
		constexpr size_t operator()(size_t el_pos, size_t val_pos) {
			return val_pos  * 9 + el_pos;
		}
	};

	template<>
	struct View_value_locator<View_type::region> {
		 constexpr size_t operator()(size_t el_pos, size_t val_pos) {
			return (27 * (el_pos /3) + 3 * (el_pos % 3)) + (9 * (val_pos/3) + (val_pos % 3));
		}
	};

	template<>
	struct View_value_locator<View_type::cell> {
		constexpr size_t operator()(size_t el_pos, size_t val_pos) {
			return (27 * (val_pos / 3) + 3 * (val_pos % 3)) + el_pos;
		}
	};

	struct Sub_view {
		
		using value_type = Grid::value_type;
		using iterator = Elem_iter<Sub_view, value_type>;
		using locator_type = std::function<size_t(size_t, size_t)>;
		Sub_view(Grid& gd, locator_type loc, size_t pos) :m_gd{ gd }, m_loc{ loc }, m_pos{ pos } {}

		value_type& operator[](size_t n) {
			return m_gd[m_loc(m_pos, n)];
		}

		iterator begin() {
			return iterator(*this);
		}

		std::default_sentinel_t end() {
			return {};
		}

		size_t m_pos;
		locator_type m_loc;
		Grid& m_gd;
	};

	template<View_type T>
	Sub_view make_sub_view(Grid& gd, size_t pos) {
		return Sub_view{ gd, View_value_locator<T>{}, pos };
	}
	
	struct View {
		using value_type = Grid::value_type;
		using iterator = Elem_iter<View, Sub_view, Sub_view>;
		using locator_type = std::function<size_t(size_t, size_t)>;
	
		View(Grid& gd, locator_type loc) :m_gd{ gd }, m_loc{loc} {}

		View& operator=(const View& other) {
			if (this != &other) {
				m_gd = other.m_gd;
				m_loc = other.m_loc;
			}
			return *this;
		}

		value_type& operator()(size_t el_pos, size_t val_pos) {
			return m_gd[m_loc(el_pos, val_pos)];
		}

		Sub_view operator[](size_t n) {
			return Sub_view{ m_gd, m_loc, n };
		}

		iterator begin() {
			return iterator(*this);
		}

		std::default_sentinel_t end() {
			return {};
		}

		locator_type m_loc;
		Grid& m_gd;
	};

	template<View_type T>
	View make_view(Grid& gd) {
		return View{ gd, View_value_locator<T>{}};
	}


	struct Validator {
		bool validate(Grid& result, Grid& data);
		bool validate(Sub_view result, Sub_view data);

		/*
		* skip invalid result
		*/
		bool validate_again(Sub_view result, Sub_view data);
	};


	std::ostream& operator<<(std::ostream& os, const Grid& t);

	std::ostream& operator<<(std::ostream& os, View v);

	std::ostream& operator<<(std::ostream& os, Sub_view v);


}