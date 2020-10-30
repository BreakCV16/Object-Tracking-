#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	//도로 주행 영상
	VideoCapture DrivingVideo("data/video.mp4");

	Mat frame;

	if (!DrivingVideo.isOpened())
	{
		cout << "Data can not open" << endl;
		return -1;
	}
	VideoWriter writer;

	namedWindow("TestSample");

	while(1)
	{
		DrivingVideo >> frame;
		if (frame.empty())
			break;
		imshow("TestSample", frame);
		if (waitKey(10) > 0)
			break;
	}
	return 0;
}

