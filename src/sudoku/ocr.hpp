#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <tesseract/baseapi.h>
#include <memory>
#include <vector>
//#include <format>
//#include <iostream>

namespace jkk::ocr {
	
	using Lines_container = std::vector<cv::Vec4i>;

	struct Line_processor {

		int operator()(const cv::Mat& src, cv::Mat& out_bin, Lines_container& h_lines, Lines_container& v_lines);
	
	private:
		void merge(const Lines_container& sorted, Lines_container& merged, const cv::Vec2i& axis, int px_threshold);

		void find(cv::Mat& im_bin, Lines_container& lines, cv::Size size, double min_len, double min_thick);

		Lines_container result_lines;
		cv::Mat im_horizon;
		cv::Mat im_vertical;
	};

	struct Cell_view {

		struct iterator
		{
			using value_type = cv::Rect;
			using reference = value_type;
			void operator++()
			{
				if (++col == col_end) {
					++row;
					col = 0;
				}

			}
			reference operator*()
			{
				return m_view.at(row, col);
			}
			bool operator==(std::default_sentinel_t)
			{
				return row >= row_end;
			}

			explicit iterator(Cell_view& view) :
				m_view{ view }, row{ 0 }, row_end{ view.h.size() - 1 }, col{ 0 }, col_end{ view.v.size() - 1 }
			{
			}

		private:
			size_t row;
			size_t row_end;
			size_t col;
			size_t col_end;
			Cell_view& m_view;
		};

		Cell_view(const Lines_container& hl, const Lines_container& vl) :h{ hl }, v{vl} {}

		cv::Rect at(size_t row, size_t col) {
			return {v[col][0], h[row][1], v[col + 1][0] - v[col][0], h[row + 1][1] - h[row][1] };
		}

		iterator begin() {
			return iterator(*this);
		}

		std::default_sentinel_t end() {
			return {};
		}

		const Lines_container& h;
	    const Lines_container& v;
	};


	using Ocr_str = std::shared_ptr<char[]>;
	struct Ocr {
		
		int prepare_api(tesseract::TessBaseAPI& api);

		Ocr_str operator()(tesseract::TessBaseAPI& api, cv::Mat& im);
	};

}