#include <iostream>
#include <opencv2/opencv.hpp>
#include "EdgeExtract.h"
#include "Converter.h"
#include "Region.h"
#include "Drawlines.h"

using namespace cv;
using namespace std;
//
//Hough Transform �Ķ����
float rho = 2; // distance resolution in pixels of the Hough grid
float theta = 1 * CV_PI / 180; // angular resolution in radians of the Hough grid
float hough_threshold = 15;	 // minimum number of votes(intersections in Hough grid cell)
float minLineLength = 10; //minimum number of pixels making up a line
float maxLineGap = 20;	//maximum gap in pixels between connectable line segments
//

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
		//imshow("Extract edge", img_edges);


		UMat uImage_edges;
		img_edges.copyTo(uImage_edges);

		//5. ���� ������ ����(�� ������ ������ǥ�� ����ǥ�� �����)
		vector<Vec4i> lines;
		HoughLinesP(uImage_edges, lines, rho, theta, hough_threshold, minLineLength, maxLineGap);
		//�׷��� ������ �������� ��ȯ�Ͽ� Edge ����
		

		Mat img_line = Mat::zeros(frame.rows, frame.cols, CV_8UC3);
		draw_line(img_line, lines);
		
		Mat img_annotated;
		//7. ���� ���� 6���� ������ ���� ������ 
		addWeighted(frame, 0.8, img_line, 1.0, 0.0, img_annotated);
		//imshow("img_annotated", img_annotated);
		
		//9. ����� ȭ�鿡 ������ 
		Mat img_result;
		resize(img_annotated, img_annotated, Size(frame.rows * 0.7, frame.cols * 0.7));
		resize(img_edges, img_edges, Size(frame.rows * 0.7, frame.cols * 0.7));
		cvtColor(img_edges, img_edges, COLOR_GRAY2BGR);
		hconcat(img_edges, img_annotated, img_result);
		imshow("��ģ��", img_result);
		

		if (waitKey(10) > 0)
			break;
	}

	//writer.release();
	return 0;
}
