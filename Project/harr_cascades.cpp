#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

Point findCenter(Rect rect);
bool intersect(Point lineP1, Point lineP2, Point rectCenter);

int main(int argc, char* argv[]) {
	const String fileName = "new_video.avi";
	VideoCapture capture(fileName);

	if (!capture.isOpened())
	{
		cout << "Error loading the video." << endl;
		int error = -1;
		return 1;
	}

	//Load the car cascades
	String car_cascade_name = "haarcascade_cars.xml";
	CascadeClassifier car_cascade;

	if (!car_cascade.load(car_cascade_name))
	{
		printf("Error : loading the cascade.\n"); return -1;
	}

	Mat frame;
	int numCars = 0;
	capture.read(frame);

	while (capture.isOpened()) {
		capture.read(frame);
		Mat drawingFrame = frame.clone();

		vector<Rect> cars;
		car_cascade.detectMultiScale(frame, cars, 1.1, 2);

		for (int c = 0; c < cars.size(); c++) {
			Point rectCenterPt = findCenter(cars[c]);
			Rect centerRect(rectCenterPt.x, rectCenterPt.y, cars[c].width / 12, cars[c].height / 12);
			Mat thisCar = frame(centerRect);
			Scalar average = mean(thisCar);

			circle(drawingFrame, rectCenterPt, 3, (255, 255, 0), -1);
			rectangle(drawingFrame, Point(cars[c].x, cars[c].y), Point(cars[c].x + cars[c].width, cars[c].y + cars[c].height), average, 2);

		}

		imshow("Video", drawingFrame);

		char key = waitKey(33);
		if (key == 'q')
		{
			cout << "Final count: " << numCars << endl;
			break;
		}
	}

	destroyAllWindows();
}

Point findCenter(Rect rect) {
	return Point(rect.x + rect.width / 2, rect.y + rect.height / 2);
}

bool intersect(Point lineP1, Point lineP2, Point rectCenter) {
	double slope = (lineP1.y - lineP2.y) / (lineP1.x - lineP2.x);
	double yIntersect = (slope * lineP1.x) + lineP1.y;
	return rectCenter.y == ((slope * rectCenter.x) + yIntersect);
}