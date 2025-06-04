#include "alg.hpp"

#include <bitset>

namespace jkk::sudoku::alg {

	std::ostream& operator<<(std::ostream& os, Candidate m)
	{
	/*	for (size_t i = 0, cur = 0; i < 9; ++i) {
			Candidate::value_type mask = 1 << i;
			if ((m.data & mask) == mask) {
				os << ()
			}
		}*/
		return os << std::bitset<9>(m.data);
	}

	int Backtrack::operator()(Grid& result, Grid& input)
	{

		while (!result.is_full()) {
			result = input;
		}
		return 0;
	}

	bool Backtrack::is_valid(Grid& in)
	{
		View view = make_view<View_type::row>(in);
		
		for (auto sub:view) {
			if (!is_valid(sub)) {
				return false;
			}
		}
		view = make_view<View_type::col>(in);

		for (auto sub : view) {
			if (!is_valid(sub)) {
				return false;
			}
		}

		view = make_view<View_type::region>(in);

		for (auto sub : view) {
			if (!is_valid(sub)) {
				return false;
			}
		}

		return true;
	}

	bool Backtrack::is_valid(Sub_view data)
	{
		for (size_t i = 0; i < 9; i++) {
			if (data[i] == 0) {
				continue;
			}
			for (size_t s = 0; s < 9; s++) {
				if (s == i) {
					continue;
				}
				if (data[s] == data[i]) {
					return false;
				}
			}
		}
		return true;
	}
	
}