#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include "EdgeExtract.h"
#include "Converter.h"
#include "Region.h"
#include "Drawlines.h"
#include "HaarCascades.h"

using namespace cv;
using namespace std;
//
//Hough Transform �Ķ����
float rho = 2; // distance resolution in pixels of the Hough grid
float theta = 1 * CV_PI / 180; // angular resolution in radians of the Hough grid
float hough_threshold = 15;	 // minimum number of votes(intersections in Hough grid cell)
float minLineLength = 10; //minimum number of pixels making up a line
float maxLineGap = 20;	//maximum gap in pixels between connectable line segments


int main()
{
	//���� ���� ����
	VideoCapture DrivingVideo("data/video.mp4");

	Mat frame;

	if (!DrivingVideo.isOpened())
	{
		cout << "Data can not open" << endl;
		return -1;
	}
	VideoWriter writer;
	vector<Mat> video;
	int nWidth = frame.size().width;
	int nHeight = frame.size().height;
	int cnt = 0;

	//Load the car cascades
	String car_cascade_name = "haarcascade_cars.xml";
	CascadeClassifier car_cascade;

	if (!car_cascade.load(car_cascade_name))
	{
		printf("Error : loading the cascade.\n"); return -1;
	}
	writer.open("Finael_Result", CV_FOURCC('D', 'I', 'V', 'X'), 30, Size(1920, 1080), true);
	while (1)
	{
		//���� ���� �о����
		DrivingVideo >> frame;
		//imshow("rrr", frame);
		if (frame.empty())
			break;
		/////////// haar detection�� ���� ���� ///////////
		Mat drawingFrame = frame.clone();

		//////haar cascades�� object detection ////////////
		vector<Rect> cars;
		car_cascade.detectMultiScale(frame, cars, 1.3, 2);

		for (int c = 0; c < cars.size(); c++) {
			Point rectCenterPt = findCenter(cars[c]);
			Rect centerRect(rectCenterPt.x, rectCenterPt.y, cars[c].width / 12, cars[c].height / 12);
			Mat thisCar = frame(centerRect);
			Scalar average = mean(thisCar);

			rectangle(drawingFrame, Point(cars[c].x, cars[c].y), Point(cars[c].x + cars[c].width, cars[c].y + cars[c].height), average, 3);

		}
		//imshow("Video", drawingFrame);

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
		addWeighted(img_line, 1.0, drawingFrame, 1.0, 0.0, img_annotated);
		//imshow("img_annotated", img_annotated);
		
		//9. ����� ȭ�鿡 ������ 
		Mat img_result;
		
		cvtColor(img_edges, img_edges, COLOR_GRAY2BGR);
		
		imshow("Combined Result", img_annotated);
		if (waitKey(10) > 0)
			break;

	}
	return 0;
}
