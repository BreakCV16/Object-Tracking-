#ifndef EdgeExtract_h
#define EdgeExtract_h

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat extract_colors(Mat Ori);

//*** 차선으로 인식할 범위 지정 ***//
Mat region_of_interest(Mat img_edges, int height, int width);

Mat Filtering(unsigned char* input, int width, int height, int Fsize = 3);

Mat Sobel_Vertical(unsigned char* input, int width, int height, int Fsize = 3);

Mat Sobel_Horizontal(unsigned char* input, int width, int height, int Fsize = 3);

Mat NMS(unsigned char* input, int width, int height, int Fsize = 3);

Mat DT(unsigned char* input, int width, int height, int Fsize = 3);

Mat hysteresis(unsigned char* input, int width, int height, int Fsize = 3);

void Apply_Cannyf(Mat img_filtered, Mat& img_edges);

#endif
