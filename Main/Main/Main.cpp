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

	

	while(1)
	{
		//원본 영상 읽어오기
		DrivingVideo >> frame;
		if (frame.empty())
			break;

		//미리 정해둔 흰색, 노란색 범위 내에 있는 부분만 차선 후보로 따로 저장하는거

		//그레이 스케일 영상으로 변환하여 Edge 추출
		//
		imshow("TestSample", frame);
		if (waitKey(10) > 0)
			break;
	}
	return 0;
}
/*
void color_detecting(Mat img_in, Mat& Converted_img)
{
	
}*/