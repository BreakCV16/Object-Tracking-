#include "Converter.h"
#include <opencv2/opencv.hpp>
#include <math.h>
using namespace cv;
/*
//���� ���� ���� 
Scalar lower_white = Scalar(200, 200, 200); //��� ���� (RGB)
Scalar upper_white = Scalar(255, 255, 255);
Scalar lower_yellow = Scalar(10, 100, 100); //����� ���� (HSV)
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
			//hsv(V�� �ش� �κ� )
			float delta = Cmax - Cmin;
			//delta����
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
//h: ��, s: ä��, v:���
void color_detect(Mat img_in, Mat& img_out)
{
	//������� ��� �ȼ��� ������ �͵�.
	Mat img_RGB,img_Hsv;
	img_in.copyTo(img_RGB);
	Mat img_hsv, img_combine;
	Mat white_mask, white_image;
	Mat yellow_mask, yellow_image;

	//���� ������ �����Ͽ� color�� ���� mask����.
	
	inRange(img_RGB, Scalar(200, 200, 200), Scalar(255, 255, 255), white_mask);
	bitwise_and(img_RGB, img_RGB, white_image, white_mask);
	//img_Hsv = RGB2HSV(img_RGB);
	//���� ������ RGB2HSV�Լ�
	RGB2HSV(img_RGB, img_Hsv);

	//���� ����
	inRange(img_Hsv, Scalar(30, 0.5, 0.5), Scalar(80, 1, 1), yellow_mask);
	bitwise_and(img_RGB, img_RGB, yellow_image, yellow_mask);
	//�ΰ� mask�� ������ �̹����� combine��Ų��.
	addWeighted(white_image, 1.0, yellow_image, 1.0, 0.0, img_combine);
	img_combine.copyTo(img_out);
	
	//imshow("yellow", img_out);
}
