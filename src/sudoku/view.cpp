#pragma once

#include "view.hpp"

namespace jkk::sudoku {
	std::ostream& operator<<(std::ostream& os, View v)
	{
		for (auto it : v) {
			os << it << "\n";
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& os, Sub_view v)
	{
		for (auto it : v) {
			os << static_cast<int>(it) << " ";
		}
		return os;
	}
}