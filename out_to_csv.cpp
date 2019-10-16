#include "out_to_csv.h"
#include <fstream>
#include <opencv2/core.hpp>

using std::ofstream;
using std::endl;

//write the matrix into excel;
void writetofile(cv::String filename, cv::Mat m)
{
	ofstream myfile;
	myfile.open(filename.c_str());
	myfile << cv::format(m, cv::Formatter::FMT_CSV) << endl;
	myfile.close();
}