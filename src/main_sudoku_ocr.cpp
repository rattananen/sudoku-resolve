#include "sudoku/ocr.hpp"
#include "sudoku/common.hpp"
#include "sudoku/alg.hpp"
#include "sudoku/time.hpp"
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

int do_common(jkk::sudoku::Grid& in_tb, std::ostream& os, bool draw_table) {
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

int cmd(std::string_view file_str, const char * tessdata_path) {
	cv::Mat im = cv::imread(file_str.data());

	if (im.empty()) {
		std::cerr << "cv can't decode img\n";
		return 1;
	}

	cv::Mat im_bin;
	jkk::ocr::Lines_container h_lines;
	jkk::ocr::Lines_container v_lines;

	h_lines.reserve(10);
	v_lines.reserve(10);

	jkk::ocr::Line_processor lp{};

	lp(im, im_bin, h_lines, v_lines);

	if (h_lines.size() != 10 || v_lines.size() != 10) {
		std::cerr << "no table detect.\n";
		return 1;
	}

	jkk::ocr::Cell_view view{ h_lines , v_lines };

	jkk::ocr::Ocr ocr;
	if (!ocr.init(tessdata_path, "eng")) {
		std::cerr << "can't initialize ocr program\n";
		return 1;
	}
	
	size_t i = 0;
	for (auto rect: view) {
		cv::Mat cell = im_bin(rect);

		auto str = ocr.ocr(cell);
		std::cout << (i++) << "= " << str.get() << "\n";
	}

	return 0;
}


constexpr auto help_text =
"usage:\n"
"  sudoku_ocr <img path>\n"
"    - It's require folder path to tesseract trained models (eng.traineddata) in enviroment vaiable \"TESSDATA_PREFIX\".\n"
"    - You can download trained models from https://github.com/tesseract-ocr/tessdata_best or what you prefer.\n\n"
"  sudoku_ocr <img path> <tesseract model folder>\n";

int main(int argc, char** argv) {

#ifdef _WIN32
	SetConsoleOutputCP(65001); // CP_UTF8
#endif

	if (argc == 2) {
		return cmd(argv[0], nullptr);
	}

	std::cerr << "invalid arguments\n\n" << help_text;
	return 1;
}