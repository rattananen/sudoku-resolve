#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>

// Function to merge close lines
void merge_lines(std::vector<cv::Vec4i>& lines, bool is_horizontal) {
	if (lines.empty()) {
		return;
	}

	std::sort(lines.begin(), lines.end(), [is_horizontal](const cv::Vec4i& a, const cv::Vec4i& b) {
		return (is_horizontal ? a[1] : a[0]) < (is_horizontal ? b[1] : b[0]);
		});

	std::vector<cv::Vec4i> merged_lines;
	merged_lines.push_back(lines[0]);

	for (size_t i = 1; i < lines.size(); ++i) {
		cv::Vec4i& current_line = lines[i];
		cv::Vec4i& last_merged_line = merged_lines.back();

		int current_pos = is_horizontal ? current_line[1] : current_line[0];
		int last_merged_pos = is_horizontal ? last_merged_line[1] : last_merged_line[0];

		// If the lines are close, merge them by extending the last merged line
		if (std::abs(current_pos - last_merged_pos) < 10) { // Merge threshold of 10 pixels
			last_merged_line[0] = std::min(last_merged_line[0], current_line[0]);
			last_merged_line[1] = std::min(last_merged_line[1], current_line[1]);
			last_merged_line[2] = std::max(last_merged_line[2], current_line[2]);
			last_merged_line[3] = std::max(last_merged_line[3], current_line[3]);
		}
		else {
			merged_lines.push_back(current_line);
		}
	}
	lines = merged_lines;
}

void morph_line(cv::Mat& im, std::vector<cv::Vec4i>& lines, cv::Size size) {
	cv::Mat horizontalStructure = cv::getStructuringElement(cv::MORPH_RECT, size);
	cv::erode(im, im, horizontalStructure);
	cv::dilate(im, im, horizontalStructure);

	cv::HoughLinesP(im, lines, 1, CV_PI / 180, 80, 50, 10);
}


int main(int argc, char* argv[]) {

	using Tess_str = std::unique_ptr<char[]>;
	std::string imPath{R"(D:\works\sudoku-resolve\assets\test_in.png)"};

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


	 
	cv::Mat horizontal = im_norm.clone();
	std::vector<cv::Vec4i> horizontal_lines;
	morph_line(horizontal, horizontal_lines, cv::Size(horizontal.cols - 10, 1));

	
	cv::Mat vertical = im_norm.clone();
	std::vector<cv::Vec4i> vertical_lines;
	morph_line(vertical, vertical_lines, cv::Size(1, vertical.rows - 10));


	// Create an image to draw the detected lines
	cv::Mat table_lines = im.clone();

	// Merge close lines to form a cleaner grid
	merge_lines(horizontal_lines, true);
	merge_lines(vertical_lines, false);

	// Draw horizontal lines
	for (const auto& l : horizontal_lines) {
	
		cv::line(table_lines, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
	}

	// Draw vertical lines
	for (const auto& l : vertical_lines) {
		cv::line(table_lines, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
	}


	tesseract::TessBaseAPI api;

	if (api.Init(nullptr, "eng")) {
		std::cerr << "Could not initialize tesseract.\n";
		return 1;
	}
	api.SetVariable("tessedit_char_whitelist", "0123456789");
	api.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
	api.SetImage(im_norm.data, im_norm.cols, im_norm.rows, 3, im_norm.step);


	Tess_str outText(api.GetUTF8Text());

	std::cout << "size h="  << horizontal_lines.size() << "\n";

	std::cout << "output:\n" << outText.get();
	api.End();


	auto im_norm_win = "im_norm_win";
	auto im_vert_win = "vert_line";
	auto im_tb_win = "table_lines";

	cv::imshow(im_norm_win, im_norm);
	cv::imshow(im_tb_win, table_lines);


	cv::waitKey(0);
	return 0;
}