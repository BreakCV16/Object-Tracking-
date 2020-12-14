#include <opencv2/opencv.hpp>

using namespace cv;

Point findCenter(Rect rect);
bool intersect(Point lineP1, Point lineP2, Point rectCenter);
