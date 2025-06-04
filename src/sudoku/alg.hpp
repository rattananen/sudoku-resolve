#pragma once

#include "common.hpp"
#include <inttypes.h>
#include <iostream>
#include <bit>

namespace jkk::sudoku::alg {
 
	/*
	*  use for hold 9 flags
	*/
	struct Candidate {
		using value_type = Grid::value_type;
		Candidate() = default;
		Candidate(value_type v) :data{ v } {}

		bool get_bit(size_t pos) const {
			return (data >> pos) & 1;
		}

		void set_bit(size_t pos, bool val) {
			data = (data & ~(1 << pos)) | (val << pos);
		}

		void add(value_type n) {
			data |=  1 << (n - 1);
		}

		void del(value_type n) {
			data &= ~(1 << (n - 1));
		}

		operator value_type() const { return data; }

		size_t size() const {
			return std::popcount(data);
		}

		value_type select(size_t pos) const {
			for (size_t i = 0, cur = 0;i< 9;++i) {
				value_type mask = 1 << i;
				if ((data & mask) == mask) {
					if (cur == pos) {
						return i + 1;
					}
					else {
						++cur;
					}
				}
			}
			return 0;
		}
		value_type data;
	};

	std::ostream& operator<<(std::ostream& os, Candidate m);

	struct Backtrack {
		int operator()(Grid& result, Grid& input);
		
		bool is_valid(Grid& in);
		bool is_valid(Sub_view data);
	};
}