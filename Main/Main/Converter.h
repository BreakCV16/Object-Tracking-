#ifndef Converter_h
#define Converter_h

#include <opencv2/opencv.hpp>
using namespace cv;

void color_detect(Mat img_in, Mat& img_out);
	//원하는 컬러 detect
//void RGB2HSV(Mat &src, Mat &img_out);

void RGB2HSV(Mat img_in,Mat& hsv);

#endif
