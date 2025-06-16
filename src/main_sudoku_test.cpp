#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <tesseract/baseapi.h>
#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>
#include <format>


void merge_lines(const std::vector<cv::Vec4i>& sorted, std::vector<cv::Vec4i>& merged,const cv::Vec2i& axis, int px_threshold) {
	if (sorted.empty()) {
		return;
	}

	merged.push_back(sorted[0]);

	for (size_t i = 1; i < sorted.size(); ++i) {
		auto& cur_line = sorted[i];
		auto& last_merged = merged.back();

		auto mag1 = cv::Vec2i(cur_line[0], cur_line[1]).ddot(axis); //project to axis
		auto mag2 = cv::Vec2i(last_merged[0], last_merged[1]).ddot(axis);

		if (std::abs(mag1 - mag2) < px_threshold) {
			last_merged[0] = std::min(last_merged[0], cur_line[0]);
			last_merged[1] = std::min(last_merged[1], cur_line[1]);
			last_merged[2] = std::max(last_merged[2], cur_line[2]);
			last_merged[3] = std::max(last_merged[3], cur_line[3]);
		}
		else {
			merged.push_back(cur_line);
		}
	}
}

void find_line(cv::Mat& im, std::vector<cv::Vec4i>& lines, cv::Size size, double min_len, double min_thick) {
	cv::Mat elem = cv::getStructuringElement(cv::MORPH_RECT, size);
	cv::erode(im, im, elem);
	cv::dilate(im, im, elem);

	cv::HoughLinesP(im, lines, 1, CV_PI / 180, 80, min_len, min_thick);
}

void draw_line(cv::Mat& im, std::vector<cv::Vec4i>& lines,const cv::Scalar& color) {
	for ( auto& l : lines) {
		cv::line(im, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), color, 2, cv::LINE_AA);
	}
}

int main(int argc, char* argv[]) {

	using Tess_str = std::unique_ptr<char[]>;
	std::string imPath{R"(E:\works\sudoku-resolve\assets\test_in.png)"};

	std::cout << "input img: " << imPath << "\n";

	cv::Mat im = cv::imread(imPath);

	if (im.empty()) {
		std::cerr << "cv can't decode img\n";
		return 1;
	}

	//preprocess
	cv::Mat im_bin;
	cv::cvtColor(im, im_bin, cv::COLOR_BGR2GRAY);
	cv::GaussianBlur(im_bin, im_bin, cv::Size(3, 3), 0);
	cv::threshold(im_bin, im_bin, 0,  255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
	//cv::adaptiveThreshold(im_norm, im_norm, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV, 11, 2);
	//end preprocess


	

	std::vector<cv::Vec4i> result_lines;
	result_lines.reserve(50);

	
	//horizontal

	cv::Mat im_horizon = im_bin.clone();
	find_line(im_horizon, result_lines, cv::Size(im_horizon.cols - 10, 1), 80, 10);
	
	std::vector<cv::Vec4i> h_lines;
	h_lines.reserve(10);

	std::sort(result_lines.begin(), result_lines.end(), [](const cv::Vec4i& a, const cv::Vec4i& b) {
		return a[1] < b[1];
		});

	merge_lines(result_lines, h_lines, cv::Vec2i(0,1),20);
	

	//vertical
	result_lines.clear();

	cv::Mat im_vertical = im_bin.clone();
	find_line(im_vertical, result_lines, cv::Size(1, im_vertical.rows - 10), 80, 10);

	std::vector<cv::Vec4i> v_lines;
	v_lines.reserve(10);

	std::sort(result_lines.begin(), result_lines.end(), [](const cv::Vec4i& a, const cv::Vec4i& b) {
		return a[0] < b[0];
		});

	merge_lines(result_lines, v_lines, cv::Vec2i(1, 0), 20);


	std::cout << std::format("h size={}\nv size={}\n", h_lines.size(), v_lines.size());

	if (h_lines.size() != 10 || v_lines.size() != 10) {
		std::cerr << "no table detect\n";
		return 1;
	}

	cv::Mat im_table_only = im_vertical + im_horizon;
	//cv::add(im_vertical, im_horizon, im_table_only);
	im_bin -= im_table_only;
	//cv::subtract(im_bin, im_table_only, im_bin);

	//ocr
	tesseract::TessBaseAPI api;

	if (api.Init(nullptr, "eng")) {
		std::cerr << "Could not initialize tesseract.\n";
		return 1;
	}
	api.SetVariable("tessedit_char_whitelist", "0123456789");
	
	api.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
	api.SetVariable("debug_file", "/dev/null");
	


	cv::imshow("im_norm_win", im_bin);
	cv::imshow("im_table_only", im_table_only);

	auto crop_win = "crop_win";
	cv::namedWindow(crop_win);

	for (size_t i = 0;i < 9; ++i) {
		auto& h_line = h_lines[i];
		auto& h_line_next = h_lines[i + 1];
		for (size_t j = 0; j < 9; ++j) {
			
			auto& v_line =  v_lines[j];
			auto& v_line_next = v_lines[j + 1];
			cv::Rect rect(v_line[0], h_line[1], v_line_next[0] - v_line[0], h_line_next[1] - h_line[1]);

			cv::Mat crop = im_bin(rect);

			api.SetImage(crop.data, crop.cols, crop.rows, crop.elemSize(), crop.step1());

			Tess_str outText(api.GetUTF8Text());

			std::cout << std::format("[{}, {}]={}\n",i,j,outText.get());

			cv::imshow(crop_win, crop);

			cv::waitKey(0);
		}
	}



	api.End();

	return 0;
}