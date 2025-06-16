#include "ocr.hpp"

#include <algorithm>

namespace jkk::ocr {
	void Line_processor::merge(const Lines_container& sorted, Lines_container& merged, const cv::Vec2i& axis, int px_threshold)
	{
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
	void Line_processor::find(cv::Mat& im_bin, Lines_container& lines, cv::Size size, double min_len, double min_thick)
	{
		cv::Mat elem = cv::getStructuringElement(cv::MORPH_RECT, size);
		cv::erode(im_bin, im_bin, elem);
		cv::dilate(im_bin, im_bin, elem);

		cv::HoughLinesP(im_bin, lines, 1, CV_PI / 180, 80, min_len, min_thick);
	}
	int Line_processor::operator()(const cv::Mat& src, cv::Mat& out_bin, Lines_container& h_lines, Lines_container& v_lines)
	{
		//prepare binary img
		cv::cvtColor(src, out_bin, cv::COLOR_BGR2GRAY);
		cv::GaussianBlur(out_bin, out_bin, cv::Size(3, 3), 0);
		cv::threshold(out_bin, out_bin, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

		result_lines.reserve(50);

		//process horizontal
		out_bin.copyTo(im_horizon);
		find(im_horizon, result_lines, cv::Size(im_horizon.cols - 10, 1), 80, 10);
		std::sort(result_lines.begin(), result_lines.end(), [](const cv::Vec4i& a, const cv::Vec4i& b) { return a[1] < b[1]; });
		merge(result_lines, h_lines, cv::Vec2i(0, 1), 20);

		result_lines.clear();

		//process vertical
		out_bin.copyTo(im_vertical);
		find(im_vertical, result_lines, cv::Size(1, im_vertical.rows - 10), 80, 10);
		std::sort(result_lines.begin(), result_lines.end(), [](const cv::Vec4i& a, const cv::Vec4i& b) { return a[0] < b[0]; });
		merge(result_lines, v_lines, cv::Vec2i(1, 0), 20);

		out_bin -= im_vertical + im_horizon;


		return 0;
	}



	Ocr_str Ocr::operator()(tesseract::TessBaseAPI& api, cv::Mat& im)
	{
		api.SetImage(im.data, im.cols, im.rows, im.elemSize(), im.step1());

		return Ocr_str(api.GetUTF8Text());
	}
	int Ocr::prepare_api(tesseract::TessBaseAPI& api)
	{
		api.SetVariable("tessedit_char_whitelist", "0123456789");
		api.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
		api.SetVariable("debug_file", "/dev/null");
		return 0;
	}
}