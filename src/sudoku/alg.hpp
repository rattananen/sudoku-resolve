#pragma once

#include "common.hpp"
#include <inttypes.h>
#include <iostream>
#include <bit>
#include <array>

namespace jkk::sudoku::alg {


	//aka brute force
	struct Backtrack_at_last {

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

		struct Sub_table {
			using value_type = Candidate;
			using locator_type = std::function<size_t(size_t, size_t)>;
			Sub_table(Candidate_table& tb, locator_type loc, size_t pos) :m_tb{ tb }, m_loc{ loc }, m_pos{ pos } {}

			value_type& operator[](size_t n) {
				return m_tb[m_loc(m_pos, n)];
			}
		private:
			size_t m_pos;
			locator_type m_loc;
			Candidate_table& m_tb;
		};



		template<View_type T>
		Sub_table make_sub_table(Candidate_table& tb, size_t pos) {
			return Sub_table{ tb, View_value_locator<T>{}, pos };
		}

		struct Candidate_track {
			using value_type = Grid::value_type;

			void next() {
				for (size_t i = 0;i< 81; ++i) {
					if (track_tb[i] < size_tb[i]) {
						track_tb[i]++;
						break;
					}
					else {
						track_tb[i] = 0;
					}
				}
			}
		/*	void fill(const Candidate_table& f_ctb, Grid& in) {
				for (size_t i = 0; i < 81; ++i) {
					if (size_tb[i] > 0) {
						in[i] = f_ctb[i].peek(track_tb[i]);
					}
				}
			}*/

			std::array<value_type, 81> size_tb;
			std::array<value_type, 81> track_tb;
		};

		void make_candidate_track(Candidate_track& to,const Candidate_table& tb);

		int operator()(Grid& result, Grid& input);

		template<View_type T>
		void refine(Candidate_table& out,  Grid& in);
		void refine_full(Candidate_table& ctb,  Grid& in);
		void clear(Candidate_table& ctb,  Grid& in);
		void solve_by_pick_candidate_size_1(Candidate_table& ctb, Grid& in);


		int solve_by_force(const Candidate_table& ctb_in, Grid& reuslt);
		bool is_valid(Grid& in);
		bool is_valid(Sub_view data);
	};

	std::ostream& operator<<(std::ostream& os, Backtrack_at_last::Candidate m);

	std::ostream& operator<<(std::ostream& os, const Backtrack_at_last::Candidate_table& tb);

	std::ostream& operator<<(std::ostream& os, const Backtrack_at_last::Candidate_track& tb);
}