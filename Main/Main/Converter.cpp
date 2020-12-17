#include "Converter.h"
#include <opencv2/opencv.hpp>
#include <math.h>
using namespace cv;
/*
//차선 색깔 범위 
Scalar lower_white = Scalar(200, 200, 200); //흰색 차선 (RGB)
Scalar upper_white = Scalar(255, 255, 255);
Scalar lower_yellow = Scalar(10, 100, 100); //노란색 차선 (HSV)
Scalar upper_yellow = Scalar(40, 255, 255);
*/

void RGB2HSV(Mat img_in,Mat& hsv_img) {
	int row = img_in.rows;
	int col = img_in.cols;
	hsv_img =Mat(row, col, CV_32FC3);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			float b = img_in.at<Vec3b>(i, j)[0] / 255.0;
			float g = img_in.at<Vec3b>(i, j)[1] / 255.0;
			float r = img_in.at<Vec3b>(i, j)[2] / 255.0;
			float Cmin = min(r, min(g, b));
			float Cmax = max(r, max(g, b));
			hsv_img.at<Vec3f>(i, j)[2] = Cmax; 
			//hsv(V에 해당 부분 )
			float delta = Cmax - Cmin;
			//delta설정
			float h, s;
			if (Cmax != 0) {
				s = delta / Cmax;
			}
			else {
				s = 0;
			}
			
			if (r == Cmax) {
				h = (g - b) / delta;
				
			}
			else if (g == Cmax) {
				h = 2 + (b - r) / delta;
			}
			else if(b == Cmax){
				h = 4 + (r - g) / delta;
			}
			h *= 60;
			
			
			if (h < 0)
				h += 360;
			
			hsv_img.at<Vec3f>(i, j)[0] = h;
			hsv_img.at<Vec3f>(i, j)[1] = s;
		}
	}

}
//h: 색, s: 채도, v:밝기
void color_detect(Mat img_in, Mat& img_out)
{
	//노란색과 흰색 픽셀만 검출할 것들.
	Mat img_RGB,img_Hsv;
	img_in.copyTo(img_RGB);
	Mat img_hsv, img_combine;
	Mat white_mask, white_image;
	Mat yellow_mask, yellow_image;

	//색상 범위를 제한하여 color에 대한 mask생성.
	
	inRange(img_RGB, Scalar(200, 200, 200), Scalar(255, 255, 255), white_mask);
	bitwise_and(img_RGB, img_RGB, white_image, white_mask);
	//img_Hsv = RGB2HSV(img_RGB);
	//직접 구현한 RGB2HSV함수
	RGB2HSV(img_RGB, img_Hsv);

	//범위 설정
	inRange(img_Hsv, Scalar(30, 0.5, 0.5), Scalar(80, 1, 1), yellow_mask);
	bitwise_and(img_RGB, img_RGB, yellow_image, yellow_mask);
	//두개 mask를 적용한 이미지를 combine시킨다.
	addWeighted(white_image, 1.0, yellow_image, 1.0, 0.0, img_combine);
	img_combine.copyTo(img_out);
	
	//imshow("yellow", img_out);
}
