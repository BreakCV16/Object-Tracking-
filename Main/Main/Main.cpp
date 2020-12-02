#include <iostream>
#include <opencv2/opencv.hpp>
#include "EdgeExtract.h"
#include "Converter.h"
#include "Region.h"

using namespace cv;
using namespace std;

int main()
{
	//도로 주행 영상
	VideoCapture DrivingVideo("data/challenge.mp4");

	Mat frame;

	if (!DrivingVideo.isOpened())
	{
		cout << "Data can not open" << endl;
		return -1;
	}

	int nWidth = frame.size().width;
	int nHeight = frame.size().height;
	int cnt = 0;

	while (1)
	{
		//원본 영상 읽어오기
		DrivingVideo >> frame;
		if (frame.empty())
			break;

		//2. 차선 후보 따로 저장
		//img_filtered = 차선 후보 영상
		Mat img_filtered;
		//후보영상에 따른 edge영상
		Mat img_edges;
		color_detect(frame, img_filtered);

		//3. 그레이스케일 영상으로 변환하여 에지 성분을 추출
		Apply_Cannyf(img_filtered, img_edges);
		//imshow("2번",img_edges);

		//4. 차선 검출할 영역을 제한함 (진행방향 바닥에 존재하는 차선으로 한정)
		img_edges = SetRegion(img_edges, img_filtered.rows, img_filtered.cols);

		// 4번까지 실행결과
		imshow("Extract edge", img_edges);

		//그레이 스케일 영상으로 변환하여 Edge 추출
		//imshow("TestSample", frame);

		if (waitKey(10) > 0)
			break;
	}

	//writer.release();
	return 0;
}
