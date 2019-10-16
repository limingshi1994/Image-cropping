#pragma once
#include <opencv2/core.hpp>;

using namespace cv;

cv::Mat cut_img(cv::Mat& m, const int& x, const int& y, const int& width, const int& height);