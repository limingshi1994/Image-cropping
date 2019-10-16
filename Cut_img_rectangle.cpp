#include <string>
#include <iostream>
#include <opencv2/core.hpp>
#include "Cut_img_rectangle.h"


using namespace cv;
using namespace std;

cv::Mat cut_img(cv::Mat& m, const int& x, const int& y, const int& width, const int& height)
{
	/*
	//define the upper-left crop base point;
	int x = 0;
	int y = 0;
	std::cout << "please enter the base point coordinates : " << std::endl;
	std::cout << "x : ";
	std::cin >> x;
	std::cout << "y : ";
	std::cin >> y;

	
	//define the cropping size;
	int height = 0;
	int width = 0;
	std::cout << "please enter the size of image after cropping : " << std::endl;
	std::cout << "height : ";
	std::cin >> height;
	std::cout << "width : ";
	std::cin >> width;
	*/

	//create the cropping mask;
	cv::Rect ROI(x, y, width, height);

	//cropping;
	cv::Mat croppedimg = m(ROI);

	return croppedimg;
}