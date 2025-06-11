#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <memory>
#include <iostream>

int main(int argc, char* argv[]) {

	using Tess_str = std::unique_ptr<char[]>;
	std::string imPath{R"(E:\works\sudoku-resolve\assets\test_in.png)"};

	std::cout << "input img: " << imPath << "\n";

	cv::Mat im = cv::imread(imPath);



	if (im.empty()) {
		std::cerr << "cv can't decode img\n";
		return 1;
	}

	//std::cout << "im:\n" << im << "\n";

	cv::Mat tmp_im;

	cv::cvtColor(im, tmp_im, cv::COLOR_BGR2GRAY);
	//cv::blur(tmp_im, tmp_im, cv::Size(3, 3));

	//std::cout << "tmp_im:\n" << tmp_im << "\n";

	tesseract::TessBaseAPI api;

	if (api.Init(nullptr, "eng")) {
		std::cerr << "Could not initialize tesseract.\n";
		return 1;
	}
	api.SetVariable("tessedit_char_whitelist", "0123456789");
	api.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
	api.SetImage(im.data, im.cols, im.rows, 3, im.step);


	Tess_str outText(api.GetUTF8Text());

	std::cout << "output:\n" << outText.get();
	api.End();


	auto source_window = "test1";
	cv::namedWindow(source_window);
	cv::imshow(source_window, tmp_im);
	cv::waitKey(0);
	return 0;
}