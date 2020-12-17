#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

Point findCenter(Rect rect) {
	return Point(rect.x + rect.width / 2, rect.y + rect.height / 2);
}

bool intersect(Point lineP1, Point lineP2, Point rectCenter) {
	double slope = (lineP1.y - lineP2.y) / (lineP1.x - lineP2.x);
	double yIntersect = (slope * lineP1.x) + lineP1.y;
	return rectCenter.y == ((slope * rectCenter.x) + yIntersect);
}