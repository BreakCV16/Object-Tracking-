#include <iostream>
#include <opencv2/opencv.hpp>
//#include <gsl/gsl_fit.h>

using namespace cv;
using namespace std;

Mat SetRegion(Mat img_edges, int height, int width)
{
	float trap_bottom_width = 0.85;  // 사다리꼴의 아래쪽 가장자리 너비, 이미지 너비의 백분율로 표시
	float trap_top_width = 0.07;     // 사다리꼴의 상단 가장자리 너비, 이미지 너비의 백분율로 표시
	float trap_height = 0.4;	// 사다리꼴의 높이, 이미지 높이의 백분율로 표시

	Point points[4];
	points[0] = Point((width * (1 - trap_bottom_width)) / 2, height);
	points[1] = Point((width * (1 - trap_top_width)) / 2, height - height * trap_height);
	points[2] = Point(width - (width * (1 - trap_top_width)) / 2, height - height * trap_height);
	points[3] = Point(width - (width * (1 - trap_bottom_width)) / 2, height);

	Mat img_mask = Mat::zeros(img_edges.rows, img_edges.cols, CV_8UC1);

	Scalar ignore_mask_color = Scalar(255, 255, 255);
	const Point* ppt[1] = { points };
	int npt[] = { 4 };

	fillPoly(img_mask, ppt, npt, 1, Scalar(255, 255, 255), LINE_8); // 채우기

	Mat img_masked;
	bitwise_and(img_edges, img_mask, img_masked);

	return img_masked;
}