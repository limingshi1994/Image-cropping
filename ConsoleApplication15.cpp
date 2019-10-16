#include <iostream>
#include <sstream>
#include <fstream>
#include <list>
#include <vector>
#include <string>
#include <filesystem>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utils/filesystem.hpp>
#include "out_to_csv.h"
#include "Cut_img_rectangle.h"
#include "cut_section.h"
#include "ConsoleApplication15.h"


using namespace std;
using namespace cv;
using namespace utils;
using namespace experimental::filesystem;



int main()
{
	//division bar;
	std::string bar = "********************************************\n";

	//grab all the pictures names and assign to img sequencially;
	vector<cv::String> names;

	//assign filter;
	std::cout << "use filter to select specific files : ";
	std::stringstream filter_name;
	std::string pattern;
	std::getline(std::cin, pattern);
	filter_name << '*' << pattern << '*';
	std::string filter = filter_name.str();
	std::cout << bar;

	//assign path;
	std::cout << "Enter the path of images folder : " << std::endl;
	std::string path;
	std::getline(std::cin, path);
	std::replace(path.begin(), path.end(), '\\', '/');
	std::cout << bar;

	//grab images;
	cv::utils::fs::glob(path, filter, names);

	//create a directory to store the results;
	std::stringstream result_path;
	result_path << path << "/crop";
	create_directory(result_path.str());

	//set the size of mask;
	cv::Size_<int> matsize;
	int rows = matsize.height;
	int cols = matsize.width;

	std::cout << "please enter the size of pictures (width, height) : " << std::endl;
	std::cin >> cols >> rows;
	std::cout << bar;

	//define the center of the mask circles;
	int centerx = cols / 2;
	int centery = rows / 2;
after_center_change:
	const cv::Point center(centerx, centery);

	//build the mask with type CV_8UC! and all element black;
	cv::Mat mask(rows, cols, CV_8UC1, Scalar(0));

	//define the mask circle radius and fill, then draw;
	cout << "Enter the inner and outer radius of the ring : " << std::endl;
	int inner, outer;
	cin >> inner >> outer;
	cout << bar;

	//draw the circles;
	cv::circle(mask, center, outer, Scalar(255), -1);
	cv::circle(mask, center, inner, Scalar(0), -1);

	//convert the mask to CV8UC1;
	cv::Mat converted_mask;

	mask.convertTo(converted_mask, CV_8UC1);

	//jpeg image quality flag;
	vector<int> jpegflag;
	jpegflag.push_back(IMWRITE_JPEG_QUALITY);
	jpegflag.push_back(100);

	//write mask into file;
	//output the mask;
	cv::namedWindow("ring_mask", CV_WINDOW_NORMAL);
	cv::resizeWindow("ring_mask", cols / 10, rows / 10);
	cv::imshow("ring_mask", converted_mask);
	cv::waitKey(5);
	cv::imwrite("mask.jpg", converted_mask, jpegflag);

	//reading experimental image;
	cv::Mat img;
	cv::Mat res;

	//define the size of vector name_size;
	vector<cv::String>::size_type names_size;
	names_size = names.size();

	//define the prefix and suffix of file name;
	stringstream crop_path;
	crop_path << result_path.str() << "/cropped_";
	cv::String prefix = crop_path.str();
	cv::String suffix = ".jpg";
	cv::String csvsuffix = ".csv";
	stringstream ss;

	// get the triangle mask;
	cv::Mat t_mask;
	t_mask = section(center, rows, cols);

	//combine ring_mask with triangle_mask;
	cv::Mat final_mask;
	bitwise_and(converted_mask, converted_mask, final_mask, t_mask);

	//show final_mask;
	cv::namedWindow("fin_mask", CV_WINDOW_NORMAL);
	cv::resizeWindow("fin_mask", cols / 10, rows / 10);
	cv::imshow("fin_mask", final_mask);
	cv::waitKey(5);

	//contour array of array establish;
	std::vector<std::vector<cv::Point>> final_mask_contour;

	//pick a picture to draw contour;
	img = cv::imread(names[0]);

	//find contour of mask to get an area for mean value calculation
	cv::findContours(final_mask, final_mask_contour, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	cv::drawContours(img, final_mask_contour, 0, cv::Scalar(0, 0, 255), 10, 4);
	cv::namedWindow("fin_mask_show", CV_WINDOW_NORMAL);
	cv::resizeWindow("fin_mask_show", cols / 10, rows / 10);
	cv::imshow("fin_mask_show", img);
	cv::waitKey(5);

	//Ask user if this cropping area is good;
	cout << "Would you like to change the center of the cropping zone ? (Y/N) : ";
	char center_change;
center_reenter:
	std::cin >> center_change;
	if (center_change == 'Y')
	{
		cout << "please enter the change of x and y (+-n) : " << endl;
		cout << "x : ";
		int delta_x;
		std::cin >> delta_x;
		int newx = center.x + delta_x;
		cout << "y : ";
		signed int delta_y;
		std::cin >> delta_y;
		int newy = center.y + delta_y;
		centerx = newx;
		centery = newy;
		cout << bar;
		goto after_center_change;
	}
	else if ((center_change != 'N') && (center_change != 'Y'))
	{
		std::cout << "please reenter : ";
		cout << bar;
		goto center_reenter;
	}
	else
	{
		std::cout << bar;
	}

	//create a list to store the ROI mean value of picture.
	std::list<double> ROIS;

	//Ask the user if want to crop the image and initialize size data;
	std::cout << "Would you like to crop the image in rectangle ? (Y/N) : ";
	char token; 
	int x = 0;
	int y = 0;
	int height = 0;
	int width = 0;

re_enter:
	if ((!(std::cin >> token)) || ((token != 'Y') && (token != 'N')))
	{
		std::cout << bar;
		std::cout << "Please re-enter : ";
		std::cin.clear();
		goto re_enter;
	}
	else if (token == 'Y')
	{
		//define the upper-left crop base point;
		std::cout << "please enter the base point coordinates : " << std::endl;
		std::cout << "x : ";
		std::cin >> x;
		std::cout << "y : ";
		std::cin >> y;


		//define the cropping size;
		std::cout << "please enter the size of image after cropping : " << std::endl;
		std::cout << "height : ";
		std::cin >> height;
		std::cout << "width : ";
		std::cin >> width;
		std::cout << bar;
	}
	else
		std::cout << bar;


	//ask if ouput to csv;
	cout << "would you like to output to excel ? (Y/N) : ";
	char token2;
re_enter2:
	if ((!(std::cin >> token2)) || ((token2 != 'Y') && (token2 != 'N')))
	{
		std::cout << bar;
		std::cout << "Please re-enter : ";
		std::cin.clear();
		goto re_enter2;
	}
	else
		std::cout << bar;

	//assign name[i] to img;
	for (vector<cv::String>::size_type i = 0; i != names_size; ++i)
	{
		//assign filenames to each file;
		cout << "start processing picture..." << endl;
		cout << bar;
		img = cv::imread(names[i]);
		ss << prefix << i + 1 << suffix;
		cv::String filename = ss.str();
		ss.str("");

		//convert source image;
		cv::Mat converted_img;
		cvtColor(img, img, CV_BGRA2GRAY);
		img.convertTo(converted_img, CV_8UC1);

		//mean value storage;
		cv::Scalar v_mean(0);
		v_mean = cv::mean(converted_img, final_mask);
		std::cout << "The mean value of ROI is : " << v_mean.val[0] << endl;
		ROIS.push_back(v_mean.val[0]);
		std::cout << bar;

		//bitwise to compare and overlay mask on pictures;
		cv::bitwise_and(converted_img, converted_img, res, final_mask);

		//create another mask to fit the picture after cut;
		cv::Mat final_mask2;

		//Ask the user if want to crop the image;
		if (token == 'Y')
		{
			res = cut_img(res, x, y, width, height);
			final_mask2 = final_mask;
		}
		else
		{
			goto next;
		}

	next:
		//output files;
		cv::imwrite(filename, res, jpegflag);

		//ask if ouput to csv;
		if (token2 == 'Y')
		{
			//auto crop;
			cv::Rect boundbox;
			if (token == 'Y')
			{
				final_mask2 = cut_img(final_mask2, x, y, width, height);
				boundbox = cv::boundingRect(final_mask2);
			}
			else
			{
				boundbox = cv::boundingRect(final_mask);
			}
			res = res(boundbox);

			//create a directory for files
			ss << prefix << i + 1 << csvsuffix;
			cv::String csvfilename = ss.str();
			ss.str("");

			//write to csv;
			cout << "converting to csv..." << endl;
			writetofile(csvfilename, res);
			cout << bar;
			cout << "processed : " << endl << filename << endl;
			cout << bar;
		}
		else
		{
			cout << bar;
			continue;
		}
	}

	//print the list of ROI average numbers into file;
	std::cout << "printing results to txt file..." << std::endl;
	std::cout << bar;
	result_path << "/Average.txt";
	std::ofstream write_ave;
	write_ave.open((result_path.str()));
	std::list<double>::iterator print_itr = ROIS.begin();
	for (; print_itr != ROIS.end(); print_itr++)
	{
		write_ave << *print_itr << std::endl;
	}

	cout << "finished!" << endl;
}