#include "sudoku/common.hpp"
#include "sudoku/alg.hpp"
#include "sudoku/time.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif


int do_common(jkk::sudoku::Grid& in_tb,  std::ostream& os,  bool draw_table) {
	if (in_tb.is_bad() || !jkk::sudoku::is_ok(in_tb)) {
		std::cerr << "invalid grid\n";
		return 1;
	}

	jkk::sudoku::alg::Combine alg{};

	jkk::time::Stopwatch stopwatch;

	stopwatch.start();
	auto err = alg(in_tb);
	auto dp = stopwatch.stop();

	std::cerr << "time use: " << dp << "\n\n";

	if (err) {
		std::cerr << "can't resolve\n";
		return 1;
	}
	if (draw_table) {
		jkk::sudoku::draw_table(os, in_tb);
	}
	else {
		os << in_tb;
	}
	os << "\n";

	return 0;
}

int cmd_interactive() {
	jkk::sudoku::Grid in_tb{};

	std::string buf;

	std::cout << "insert [0-9] for 81 numbers seperate with space (no need to insert all numbers at once)\n";
	
	for (size_t pos = 0; pos < 81;) {
		std::cout << (81 - pos) << " numbers remain. position " << (pos + 1) << " is next.\n";
		std::getline(std::cin, buf);
		if (buf.empty()) {
			continue;
		}
		std::istringstream iss(buf);
		jkk::sudoku::Grid::value_type number;

		while (pos < 81 && iss >> number) {
			if (number >=0 && number <= 9) {
				in_tb[pos] = number;
				++pos;
			}
		}
	}

	std::cout << "input:\n";
	jkk::sudoku::draw_table(std::cout, in_tb);

	return do_common(in_tb, std::cout, true);
}


int cmd_file_in(std::string_view file_str, std::ostream& os, bool draw_table) {
	std::cout << file_str << "\n";
	std::ifstream ifs{ file_str.data() };
	if (!ifs.is_open()) {
		std::cerr << "can't open input file: << " << file_str.data() << "\n";
		return 1;
	}

	jkk::sudoku::Grid in_tb{};

	for (size_t n = 0; n < 81; ++n) {
		ifs >> in_tb[n];
	}

	if (ifs.fail()) {
		std::cerr << "invalid file\n";
		return 1;
	}

	return do_common(in_tb, os, draw_table);
}

int cmd_file_in_out(std::string_view file_str, std::string_view file_out_str, bool draw_table) {
	std::ofstream ofs{ file_out_str.data() };
	if (!ofs.is_open()) {
		std::cerr << "can't open output file: " << file_out_str << "\n";
		return 1;
	}

	return cmd_file_in(file_str, ofs, draw_table);
}

constexpr auto help_text =
"usage:\n"
"  sudoku_resolve --interactive\n"
"  sudoku_resolve <filepath>\n"
"  sudoku_resolve <filepath in> <filepath out>\n";


int main(int argc, const char** argv) {

#ifdef _WIN32
	SetConsoleOutputCP(65001); // CP_UTF8
#endif
	if (argc == 2 && std::string_view{ argv[1] } == "--interactive") {
		return cmd_interactive();
	}

	if (argc == 2) {
		return cmd_file_in(argv[1], std::cout, true);
	}

	if (argc == 3) {
		return cmd_file_in_out(argv[1], argv[2], false);
	}

	std::cerr << "invalid arguments\n\n" << help_text;
	return 1;
}