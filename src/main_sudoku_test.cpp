#include "sudoku/common.hpp"
#include "sudoku/alg.hpp"
#include "sudoku/time.hpp"
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

int main() {

#ifdef _WIN32
	SetConsoleOutputCP(65001); // CP_UTF8
#endif

	jkk::sudoku::Grid in_tb{
		{
	    5,3,0, 0,7,0, 0,0,0,
		6,0,0, 1,9,5, 0,0,0,
		0,9,8, 0,0,0, 0,6,0,

		8,0,0, 0,6,0, 0,0,3,
		4,0,0, 8,0,3, 0,0,1,
		7,0,0, 0,2,0, 0,0,6,

		0,6,0, 0,0,0, 2,8,0,
		0,0,0, 4,1,9, 0,0,5,
		0,0,0, 0,8,0, 0,7,9} };

	//jkk::sudoku::Grid in_tb{
	//	{
	//	5,3,4, 6,7,8, 9,1,2,
	//	6,7,2, 1,9,5, 3,4,8,
	//	1,9,8, 3,4,2, 5,6,7,

	//	8,5,9, 7,6,1, 4,2,3,
	//	4,2,6, 8,5,3, 7,9,1,
	//	7,1,3, 9,2,4, 8,5,6,

	//	9,6,1, 5,3,7, 2,8,4,
	//	2,8,7, 4,1,9, 6,3,5,
	//	3,4,5, 2,8,6, 1,7,9} };

	

	if (!is_ok(in_tb)) {
		std::cerr << "invalid input\n";
		return 1;
	}

	
	jkk::sudoku::Grid r_tb{ in_tb };

	jkk::sudoku::alg::Candidate_reduce alg{};

	jkk::time::Stopwatch stopwatch;

	stopwatch.start();

	auto err = alg(r_tb);

	auto dp = stopwatch.stop();

	std::cout 
		<< "input:\n"
		<< in_tb << "\n"
		<< "time use: " << dp << "\n";

	if (err) {
		std::cerr << "can't resolve\n";
		return 1;
	}
	

	std::cout 
		<< "result:\n"
	    << r_tb << "\n";
	

	return 0;
}