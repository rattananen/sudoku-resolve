#include "alg.hpp"

namespace jkk::sudoku::alg {

	void Backtrack_at_last::make_candidate_track(Candidate_track& to, const Candidate_table& tb)
	{

		for (size_t i = 0; i < 81; ++i) {
			to.size_tb[i] = tb[i].size();
		}
	}

	int Backtrack_at_last::operator()(Grid& result, Grid& input)
	{
		Candidate_table ctb{};

		result = input;
		refine_full(ctb, result);



		/*while (ctb.has_size_1()) {
			solve_by_pick_candidate_size_1(ctb, result);
			refine_full(ctb, result);
		}*/



		/*	if (ctb.candidate_size() == 0) {
				return  0;
			}*/


		return solve_by_force(ctb, result);
	}

	template<View_type T>
	void Backtrack_at_last::refine(Candidate_table& out, Grid& in)
	{
		for (size_t i = 0; i < 9; ++i) {
			Candidate cd{ 0 };
			auto st = make_sub_table<T>(out, i);
			auto sv = make_sub_view<T>(in, i);
			for (size_t n = 0; n < 9; ++n) {
				if (sv[n] != 0) {
					cd.push(sv[n]);
				}
			}
			for (size_t n = 0; n < 9; ++n) {
				st[n].unset(cd);
			}
		}
	}

	void Backtrack_at_last::refine_full(Candidate_table& out, Grid& in)
	{
		clear(out, in);
		refine<View_type::row>(out, in);
		refine<View_type::col>(out, in);
		refine<View_type::region>(out, in);
	}

	void Backtrack_at_last::clear(Candidate_table& out, Grid& in)
	{
		for (size_t i = 0; i < 81; ++i) {
			if (in[i] != 0) {
				out[i] = 0;
			}
		}
	}

	void Backtrack_at_last::solve_by_pick_candidate_size_1(Candidate_table& ctb, Grid& in)
	{
		for (size_t i = 0; i < 81; ++i) {
			if (ctb[i].size() == 1) {
				in[i] = ctb[i].peek(0);
			}
		}
	}

	int Backtrack_at_last::solve_by_force(const Candidate_table& ctb, Grid& result)
	{
		Candidate_track track{};
		make_candidate_track(track, ctb);

		
		std::cout << "solve_by_force\n" << result << "\n";
		std::cout << ctb << "\n";
	
		size_t i = 0;
		while (!is_valid(result)) {
			for (size_t i = 0; i < 81; ++i) {
				if (size_tb[i] > 0) {
					in[i] = f_ctb[i].peek(track_tb[i]);
				}
			}
			track.next();
			if (i++ == 10000) {
				break;
			}
			
		}

		std::cout << track << "\n";
		return 0;
	}

	bool Backtrack_at_last::is_valid(Grid& in)
	{
		View view = make_view<View_type::row>(in);

		for (auto sub : view) {
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

	bool Backtrack_at_last::is_valid(Sub_view data)
	{
		for (size_t i = 0; i < 9; i++) {
			if (data[i] == 0) {
				return false;
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



	std::ostream& operator<<(std::ostream& os, Backtrack_at_last::Candidate m)
	{
		for (size_t i = 0; i < 9; ++i) {
			Backtrack_at_last::Candidate::value_type mask = 1 << i;
			if ((m.data & mask) == mask) {
				os << (i + 1);
			}
			else {
				os << "_";
			}
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const Backtrack_at_last::Candidate_table& tb)
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
	std::ostream& operator<<(std::ostream& os, const Backtrack_at_last::Candidate_track& tb)
	{
		for (size_t i = 0; i < 81; ++i) {
			os << i << '-' << tb.size_tb[i] << ':' << tb.track_tb[i] << " ";
			auto pos = i + 1;
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
}