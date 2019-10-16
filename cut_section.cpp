#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/core/types.hpp>
#include <math.h>


//division bar;
std::string bar = "********************************************\n";

cv::Mat section(const cv::Point& STPoint, const int& rows, const int& cols)
{
	//input an empty mat to create a template for cropping;
	cv::Mat temp(rows, cols, CV_8UC1, cv::Scalar(0));

	//we need two points for line1 and two points for line2;
	//and the start points for line1 and line2 are the same which is STPoint;
	
	//length of the line;
	int l(cols);

	//angle of two lines;
	int theta_one;
	int theta_two;

	//ask for angle input;	
	std::cout << "please enter the left and right boundary angles : " << std::endl;
	while (std::cin >> theta_one >> theta_two)
	{
		if (!std::cin)
		{
			std::cout << bar;
			std::cout << " please re-enter : " << std::endl;
			continue;
		}
		else
		{
			std::cout << bar;
			break;
		}
	}

	//calculate L1P and L2P;
	cv::Point l1p;
	cv::Point l2p;

	l1p.x = STPoint.x + l * std::cos(theta_one * (CV_PI / 180));
	l2p.x = STPoint.x + l * std::cos(theta_two * (CV_PI / 180));

	l1p.y = STPoint.y + (-l) * std::sin(theta_one * (CV_PI / 180));
	l2p.y = STPoint.y + (-l) * std::sin(theta_two * (CV_PI / 180));

	//Array of points;
	std::vector<cv::Point> vec = { STPoint, l1p, l2p };
	std::vector<std::vector<cv::Point>> contours;
	contours.push_back(vec);

	//cut the picture;
	cv::drawContours(temp, contours, -1, cv::Scalar(255), cv::FILLED, 4);

	//output the mask;
	cv::namedWindow("section_mask", CV_WINDOW_NORMAL);
	cv::resizeWindow("section_mask", cols/10, rows/10);
	cv::imshow("section_mask", temp);
	cv::waitKey(5);

	//new result Mat;
	cv::Mat triangle_mask;
	triangle_mask = temp;

	return triangle_mask;
}

