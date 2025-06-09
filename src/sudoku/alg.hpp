#pragma once

#include "common.hpp"
#include <inttypes.h>
#include <iostream>
#include <bit>
#include <array>
#include <functional>

namespace jkk::sudoku::alg {

	struct Candidate_reduce {

		struct Candidate {
			using value_type = Grid::value_type;
			Candidate() :data{ 0b1'1111'1111 } {}
			Candidate(value_type v) :data{ v } {}

			operator value_type() const { return data; }

			size_t size() const {
				return std::popcount(data);
			}
			void set_all() {
				data = 0b1'1111'1111;
			}
			void set(size_t n) {
				data |= 1 << n;
			}
			void clear(size_t n) {
				data &= ~(1 << n);
			}
			void del(value_type n) {
				data &= ~(1 << (n - 1));
			}
			void push(value_type n) {
				data |= 1 << (n - 1);
			}
			void unset(Candidate c) {
				data &= ~c.data;
			}
			value_type pop() {
				for (size_t i = 0, cur = 0; i < 9; ++i) {
					value_type mask = 1 << i;
					if ((data & mask) == mask) {
						clear(i);
						return i + 1;
					}
				}
				return 0;
			}
			value_type peek(size_t pos) const {
				for (size_t i = 0, cur = 0; i < 9; ++i) {
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
		struct Candidate_table {
			Candidate& operator[](size_t n) {
				return data[n];
			}

			const Candidate& operator[](size_t n) const {
				return data[n];
			}

			Candidate& operator()(size_t row, size_t col) {
				return data[row * 9 + col];
			}

			const Candidate& operator()(size_t row, size_t col) const {
				return data[row * 9 + col];
			}

			size_t candidate_size() const {
				size_t n{};
				for (auto v : data) {
					n += v.size();
				}
				return n;
			}
			bool has_size_1() {
				for (auto v:data) {
					if (v.size() == 1) {
						return true;
					}
				}
				return false;
			}
			std::array<Candidate, 81> data;
		};

		

		using locator_type = std::function<size_t(size_t, size_t)>;

		int operator()(Grid& result);

		void refine_as(Candidate_table& out, const Grid& in, locator_type fn);
		void refine_full(Candidate_table& ctb, const Grid& in);
		void clear(Candidate_table& ctb, const  Grid& in);
		void do_solve(Candidate_table& ctb, Grid& in);

	};

	struct Backtrack {
		int operator()(Grid& result);

	//private:
		bool do_solve(Grid& result);
		bool is_ok(const Grid& in, size_t row, size_t col, Grid::value_type num);
	};

	struct Combine {
		int operator()(Grid& result);
	};

	std::ostream& operator<<(std::ostream& os, Candidate_reduce::Candidate m);

	std::ostream& operator<<(std::ostream& os, const Candidate_reduce::Candidate_table& tb);

}