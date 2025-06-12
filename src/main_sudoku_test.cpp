#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>
#include <format>


void merge_lines(std::vector<cv::Vec4i>& lines, std::vector<cv::Vec4i>& merged,  bool is_horizontal) {
	if (lines.empty()) {
		return;
	}

	std::sort(lines.begin(), lines.end(), [is_horizontal](const cv::Vec4i& a, const cv::Vec4i& b) {
		return (is_horizontal ? a[1] : a[0]) < (is_horizontal ? b[1] : b[0]);
		});

	//std::vector<cv::Vec4i> merged_lines;
	merged.push_back(lines[0]);

	for (size_t i = 1; i < lines.size(); ++i) {
		const cv::Vec4i& current_line = lines[i];
		cv::Vec4i& last_merged_line = merged.back();


		int current_pos = is_horizontal ? current_line[1] : current_line[0];
		int last_merged_pos = is_horizontal ? last_merged_line[1] : last_merged_line[0];

		// If the lines are close, merge them by extending the last merged line
		if (std::abs(current_pos - last_merged_pos) < 20) { // Merge threshold of 10 pixels
			last_merged_line[0] = std::min(last_merged_line[0], current_line[0]);
			last_merged_line[1] = std::min(last_merged_line[1], current_line[1]);
			last_merged_line[2] = std::max(last_merged_line[2], current_line[2]);
			last_merged_line[3] = std::max(last_merged_line[3], current_line[3]);
		}
		else {
			merged.push_back(current_line);
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

	cv::Mat im_norm;

	cv::cvtColor(im, im_norm, cv::COLOR_BGR2GRAY);
	cv::GaussianBlur(im_norm, im_norm, cv::Size(3, 3), 0);
	cv::threshold(im_norm, im_norm, 0,  255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
	//cv::adaptiveThreshold(im_norm, im_norm, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV, 11, 2);

	cv::Mat im_with_table = im.clone();

	cv::Mat im_line_process;
	std::vector<cv::Vec4i> result_lines;
	result_lines.reserve(50);

	

	im_norm.copyTo(im_line_process);
	find_line(im_line_process, result_lines, cv::Size(im_line_process.cols - 10, 1), 80, 10);
	
	std::vector<cv::Vec4i> h_lines;
	h_lines.reserve(10);

	merge_lines(result_lines, h_lines, true);
	draw_line(im_with_table, h_lines, cv::Scalar(0, 255, 0));

	result_lines.clear();
	
	im_norm.copyTo(im_line_process);
	find_line(im_line_process, result_lines, cv::Size(1, im_line_process.rows - 10), 80, 10);

	std::vector<cv::Vec4i> v_lines;
	v_lines.reserve(10);

	merge_lines(result_lines, v_lines, false);
	draw_line(im_with_table, v_lines, cv::Scalar(0, 0, 255));

	tesseract::TessBaseAPI api;

	if (api.Init(nullptr, "eng")) {
		std::cerr << "Could not initialize tesseract.\n";
		return 1;
	}
	api.SetVariable("tessedit_char_whitelist", "0123456789");
	api.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);


	api.SetImage(im_norm.data, im_norm.cols, im_norm.rows, im_norm.elemSize(), im_norm.step1());


	Tess_str outText(api.GetUTF8Text());

	
	std::cout << "output:\n" << outText.get();
	api.End();


	auto im_norm_win = "im_norm_win";
	auto im_vert_win = "vert_line";
	auto im_tb_win = "table_lines";

	cv::imshow(im_norm_win, im_norm);
	cv::imshow(im_tb_win, im_with_table);


	cv::waitKey(0);
	return 0;
}