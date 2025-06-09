#include "alg.hpp"

namespace jkk::sudoku::alg {


	int Candidate_reduce::operator()(Grid& result)
	{
		Candidate_table ctb{};

		refine_full(ctb, result);

		while (ctb.has_size_1()) {
			do_solve(ctb, result);
			refine_full(ctb, result);
		}


		if (ctb.candidate_size() == 0) {
			return  0;
		}

		return 1;
	}


	void Candidate_reduce::refine_as(Candidate_table& out, const Grid& in, Candidate_reduce::locator_type fn)
	{
		for (size_t i = 0; i < 9; ++i) {
			Candidate cd{ 0 };
			for (size_t j = 0; j < 9; ++j) {
				size_t n = fn(i, j);
				if (in[n] != 0) {
					cd.push(in[n]);
				}
			}
			for (size_t j = 0; j < 9; ++j) {
				out[fn(i, j)].unset(cd);
			}
		}
	}

	void Candidate_reduce::refine_full(Candidate_table& out,const Grid& in)
	{
		clear(out, in);
		refine_as(out, in, [](size_t row, size_t col) { return row * 9 + col; });
		refine_as(out, in, [](size_t col, size_t row) { return row * 9 + col; });
		refine_as(out, in, [](size_t sub_n, size_t n) { return (27 * (sub_n / 3) + 3 * (sub_n % 3)) + (9 * (n / 3) + (n % 3)); });
	}

	void Candidate_reduce::clear(Candidate_table& out, const Grid& in)
	{
		for (size_t i = 0; i < 81; ++i) {
			if (in[i] != 0) {
				out[i] = 0;
			}
		}
	}

	void Candidate_reduce::do_solve(Candidate_table& ctb, Grid& in)
	{
		for (size_t i = 0; i < 81; ++i) {
			if (ctb[i].size() == 1) {
				in[i] = ctb[i].peek(0);
			}
		}
	}


	std::ostream& operator<<(std::ostream& os, Candidate_reduce::Candidate m)
	{
		for (size_t i = 0; i < 9; ++i) {
			Candidate_reduce::Candidate::value_type mask = 1 << i;
			if ((m.data & mask) == mask) {
				os << (i + 1);
			}
			else {
				os << "_";
			}
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const Candidate_reduce::Candidate_table& tb)
	{
		for (size_t i = 0; i < 81; ++i) {
			auto pos = i + 1;
			os << tb[i] << ' ';
			if (pos % 3 == 0) {
				os << "  ";
			}
			if (pos % 9 == 0) {
				os << '\n';
			}
			if (pos % 27 == 0) {
				os << '\n';
			}
		}
		return os;
	}



	int Backtrack::operator()(Grid& result) {
		if (do_solve(result)) {
			return 0;
		}
		return 1;
	}

	bool Backtrack::do_solve(Grid& result)
	{
		size_t row, col;

		for (row = 0; row < 9; ++row) {
			for (col = 0; col < 9; ++col) {
				if (result(row, col) == 0) {
					goto solve;
				}
			}
		}

		return true; //nothing to solve
	solve:

		for (Grid::value_type num = 1; num <= 9; ++num) {
			if (is_ok(result, row, col, num)) {
				result(row, col) = num;
				if (do_solve(result)) {
					return true;
				}

				result(row, col) = 0; // Backtrack
			}
		}
		return false; //trigger Backtrack
	}



	bool Backtrack::is_ok(const Grid& in, size_t row, size_t col, Grid::value_type num)
	{
		for (size_t n = 0; n < 9; ++n) {
			if (in(row, n) == num) {
				return false;
			}
		}

		for (size_t n = 0; n < 9; ++n) {
			if (in(n, col) == num) {
				return false;
			}
		}

		size_t start_row = row - row % 3, start_col = col - col % 3;
		for (size_t n = 0; n < 3; ++n)
			for (size_t k = 0; k < 3; ++k)
				if (in(n + start_row, k + start_col) == num)
					return false;
		return true;
	}

	int Combine::operator()(Grid& result) {
		int err = Candidate_reduce{}(result);
		if (!err) {
			return 0;
		}
		return Backtrack{}(result);
	}
}