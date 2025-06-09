#include "sudoku/common.hpp"
#include "sudoku/alg.hpp"
#include "sudoku/time.hpp"
#include <iostream>
#include <string>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#endif


int cmd_file_in(std::string_view file_str, std::ostream& os) {
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
		std::cerr << "invalid value\n";
		return 1;
	}

	if (!is_ok(in_tb)) {
		std::cerr << "invalid input\n";
		return 1;
	}


	jkk::sudoku::alg::Combine alg{};

	jkk::time::Stopwatch stopwatch;

	stopwatch.start();
	auto err = alg(in_tb);
	auto dp = stopwatch.stop();

	std::cerr << "time use: " << dp << "\n";

	if (err) {
		std::cerr << "can't resolve\n";
		return 1;
	}

	os << in_tb << "\n";

	return 0;
}

int cmd_file_in_out(std::string_view file_str, std::string_view file_out_str) {
	std::ofstream ofs{ file_out_str.data() };
	if (!ofs.is_open()) {
		std::cerr << "can't open output file: " << file_out_str << "\n";
		return 1;
	}

	return cmd_file_in(file_str, ofs);
}

constexpr auto help_text =
"usage:\n"
" sudoku_resolve <filepath>\n"
" sudoku_resolve <filepath in> <filepath out>\n";


int main(int argc, const char** argv) {

#ifdef _WIN32
	SetConsoleOutputCP(65001); // CP_UTF8
#endif
	if (argc == 2) {
		return cmd_file_in(argv[1], std::cout);
	}

	if (argc == 3) {
		return cmd_file_in_out(argv[1], argv[2]);
	}

	std::cerr << "invalid arguments\n" << help_text;
	return 1;
}