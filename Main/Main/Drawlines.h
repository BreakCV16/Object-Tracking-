#ifndef Drawlines_h
#define Drawlines_h


#include <opencv2/opencv.hpp>  
#include <opencv2/imgproc.hpp>
#include <gsl/gsl_fit.h>
#include <iostream>  

using namespace cv;
using namespace std;

//hough Trans�� ���� ���� �׸���
void draw_line(Mat& img_line, vector<Vec4i> lines);


#endif
