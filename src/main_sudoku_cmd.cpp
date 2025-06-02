#include "sudoku/common.hpp"
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

int main() {

#ifdef _WIN32
	SetConsoleOutputCP(65001); // CP_UTF8
#endif

	jkk::sudoku::Grid tb{
		{
	    5,3,4,6,7,8,9,1,2,
		6,7,2,1,9,5,3,4,8,
		1,9,8,3,4,2,5,6,7,
		8,5,9,7,6,1,4,2,3,
		4,2,6,8,5,3,7,9,1,
		7,1,3,9,2,4,8,5,6,
		9,6,1,5,3,7,2,8,4,
		2,8,7,4,1,9,6,3,5,
		3,4,5,2,8,6,1,7,9} };

	std::cout << tb << "\n";

	
	jkk::sudoku::Grid_validator vd{};
	jkk::sudoku::Grid_validator::Result res{};

	//vd.test<jkk::sudoku::Grid_view_type::region>();
	vd.validate_row(res, tb, 0);

	if (res) {
		std::cout << "no error\n";
	}
	else {
		std::cout << "error\n";
	}

	

	return 0;
}