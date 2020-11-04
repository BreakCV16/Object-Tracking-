#include <iostream>
#include <opencv2/opencv.hpp>
//#include <gsl/gsl_fit.h>

using namespace cv;
using namespace std;

//*** 차선 색깔 범위 ***// 
//흰색 차선 (RGB)
Scalar white_Max = Scalar(255, 255, 255);
Scalar white_min = Scalar(200, 200, 200);
// 노란색 차선(HSV)
Scalar yellow_Max = Scalar(40, 255, 255);
Scalar yellow_min = Scalar(10, 100, 100);

//*** 영상의 흰색과 노란색 부분만 남기는 과정 ***//
Mat extract_colors(Mat Ori)
{
	Mat img_bgr, img_hsv, img_mixed;
	Mat white_mask, white_image, yellow_mask, yellow_image;
	Ori.copyTo(img_bgr);

	//흰색 추출
	inRange(img_bgr, white_min, white_Max, white_mask);
	bitwise_and(img_bgr, img_bgr, white_image, white_mask);

	//노란색 추출
	cvtColor(img_bgr, img_hsv, COLOR_BGR2HSV);

	inRange(img_hsv, yellow_min, yellow_Max, yellow_mask);
	bitwise_and(img_bgr, img_bgr, yellow_image, yellow_mask);

	//두 색의 결과를 합치는 과정
	addWeighted(white_image, 1.0, yellow_image, 1.0, 0.0, img_mixed);

	return img_mixed;
}

//*** 차선으로 인식할 범위 지정 ***//
Mat region_of_interest(Mat img_edges, int height, int width)
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

	int nWidth = frame.size().width;
	int nHeight = frame.size().height;
	int cnt = 0;

	while (1)
	{
		//원본 영상 읽어오기
		DrivingVideo >> frame;
		if (frame.empty())
			break;

		//미리 정해둔 흰색, 노란색 범위 내에 있는 부분만 차선 후보로 따로 저장하는거

		//2. 차선 후보 따로 저장
		Mat img_filtered;
		img_filtered = extract_colors(frame);

		// 2번까지 실행결과
		//imshow("img_filtered", img_filtered);

		//3. 그레이스케일 영상으로 변환하여 에지 성분을 추출
		Mat img_edges;
		cvtColor(img_filtered, frame, COLOR_BGR2GRAY);
		GaussianBlur(frame, frame, Size(3, 3), 0, 0);

		Canny(frame, img_edges, 50, 150);

		// 3번까지 실행결과
		//imshow("edge result", img_edges);

		int width = img_filtered.cols;
		int height = img_filtered.rows;

		//4. 차선 검출할 영역을 제한함 (진행방향 바닥에 존재하는 차선으로 한정)
		img_edges = region_of_interest(img_edges, height, width);

		// 4번까지 실행결과
		imshow("Extract edge", img_edges);

		//그레이 스케일 영상으로 변환하여 Edge 추출
		//imshow("TestSample", frame);

		if (waitKey(10) > 0)
			break;
	}
	return 0;
}
/*
void color_detecting(Mat img_in, Mat& Converted_img)
{

}*/