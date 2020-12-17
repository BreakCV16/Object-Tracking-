#include <iostream>
#include <opencv2/opencv.hpp>
//#include <gsl/gsl_fit.h>

using namespace cv;
using namespace std;

Mat SetRegion(Mat img_edges, int height, int width)
{
	Point points[4];
	points[0] = Point(0.9 * width, height);
	points[1] = Point(0.57 * width, 0.6 * height);
	points[2] = Point(0.43 * width, 0.6 * height);
	points[3] = Point(0.1 * width, height);

	Mat img_range = Mat::zeros(img_edges.rows, img_edges.cols, CV_8UC1);
	for (int py = 0; py < img_range.rows; py++) //720 (¼¼·Î)
	{
		for (int px = 0; px < img_range.cols; px++) //1280(°¡·Î)
		{
			if ((py >= 0.6 * img_range.rows) && ((22 * py + 15 * px - 17760) >= 0) && ((22 * py - 15 * px + 1440) >= 0))
				img_range.at<uchar>(py, px) = 255;
			else
			{
				img_range.at<uchar>(py, px) = 0;
			}
		}
	}

	Mat img_masked;

	bitwise_and(img_edges, img_range, img_masked);

	return img_masked;
}