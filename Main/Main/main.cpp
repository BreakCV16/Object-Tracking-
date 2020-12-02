#include <iostream>
#include <opencv2/opencv.hpp>
#include "EdgeExtract.h"
#include "Converter.h"
#include "Region.h"

using namespace cv;
using namespace std;

int main()
{
	//���� ���� ����
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
		//���� ���� �о����
		DrivingVideo >> frame;
		if (frame.empty())
			break;

		//2. ���� �ĺ� ���� ����
		//img_filtered = ���� �ĺ� ����
		Mat img_filtered;
		//�ĺ����� ���� edge����
		Mat img_edges;
		color_detect(frame, img_filtered);

		//3. �׷��̽����� �������� ��ȯ�Ͽ� ���� ������ ����
		Apply_Cannyf(img_filtered, img_edges);
		//imshow("2��",img_edges);

		//4. ���� ������ ������ ������ (������� �ٴڿ� �����ϴ� �������� ����)
		img_edges = SetRegion(img_edges, img_filtered.rows, img_filtered.cols);

		// 4������ ������
		imshow("Extract edge", img_edges);

		//�׷��� ������ �������� ��ȯ�Ͽ� Edge ����
		//imshow("TestSample", frame);

		if (waitKey(10) > 0)
			break;
	}

	//writer.release();
	return 0;
}
