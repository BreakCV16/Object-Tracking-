#ifndef HaarCascades_h
#define HaarCascades_h

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Point findCenter(Rect rect);
bool intersect(Point lineP1, Point lineP2, Point rectCenter);

#endif
