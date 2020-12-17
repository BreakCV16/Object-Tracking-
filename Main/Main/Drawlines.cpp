#include <opencv2/opencv.hpp>  
#include <opencv2/imgproc.hpp>
#include <gsl/gsl_fit.h>
#include <iostream>  


using namespace cv;
using namespace std;

//���ɿ��� ������ ���� �޾ƿ� Mat�����͸� �޾Ƽ� Draw�� �����ϴ� �Լ�.
float trap_bottom_width = 0.85;  // width of bottom edge of trapezoid, expressed as percentage of image width
float trap_top_width = 0.07;     // ditto for top edge of trapezoid
float trap_height = 0.4;

void draw_line(Mat& img_line, vector<Vec4i> lines)
{
	//������ �������, �����Ѵ�.
	if (lines.size() == 0) {
		cout << "No data of lines" << endl;
		return;
	}

	// In case of error, don't draw the line(s)
	bool draw_right = true;
	bool draw_left = true;
	int width = img_line.cols;
	int height = img_line.rows;


	//��� ���� ���� ���⸦ ã�´�.
	//Threshold���� ū ���⿡ ���ؼ��� ã�´�.
	float slope_threshold = 0.6;
	vector<float> slopes;
	vector<Vec4i> new_lines;

	for (int i = 0; i < lines.size(); i++)
	{
		Vec4i line = lines[i];
		int x1 = line[0];
		int y1 = line[1];
		int x2 = line[2];
		int y2 = line[3];

		//���� ����Ѵ�.
		float slope;
		if (x2 - x1 == 0) //�ڳ��� ��츦 ���
			slope = 100000.0; 
		else
			slope = (y2 - y1) / (float)(x2 - x1);


		//Filter lines based on slope
		if (abs(slope) > slope_threshold) {
			slopes.push_back(slope);
			new_lines.push_back(line);
		}
	}


	// ������ ���� �����ְ�
	//���� ��� ������ ������ ���
	vector<Vec4i> right_lines;
	vector<Vec4i> left_lines;

	for (int i = 0; i < new_lines.size(); i++)
	{

		Vec4i line = new_lines[i];
		float slope = slopes[i];

		int x1 = line[0];
		int y1 = line[1];
		int x2 = line[2];
		int y2 = line[3];


		float cx = width * 0.5; //x coordinate of center of image

		if (slope > 0 && x1 > cx && x2 > cx)
			right_lines.push_back(line);
		else if (slope < 0 && x1 < cx && x2 < cx)
			left_lines.push_back(line);
	}


	//����ȸ�� ����������
	double right_lines_x[1000];
	double right_lines_y[1000];
	float right_m, right_b;


	int right_index = 0;
	for (int i = 0; i < right_lines.size(); i++) {

		Vec4i line = right_lines[i];
		int x1 = line[0];
		int y1 = line[1];
		int x2 = line[2];
		int y2 = line[3];

		right_lines_x[right_index] = x1;
		right_lines_y[right_index] = y1;
		right_index++;
		right_lines_x[right_index] = x2;
		right_lines_y[right_index] = y2;
		right_index++;
	}
	if (right_index > 0){
		double c0, c1, cov00, cov01, cov11, sumsq;
		gsl_fit_linear(right_lines_x, 1, right_lines_y, 1, right_index,
			&c0, &c1, &cov00, &cov01, &cov11, &sumsq);
		right_m = c1;
		right_b = c0;
	}
	else {
		right_m = right_b = 1;
		draw_right = false;
	}
	//����
	double left_lines_x[1000];
	double left_lines_y[1000];
	float left_m, left_b;
	int left_index = 0;
	for (int i = 0; i < left_lines.size(); i++) {
		Vec4i line = left_lines[i];
		int x1 = line[0];
		int y1 = line[1];
		int x2 = line[2];
		int y2 = line[3];
		left_lines_x[left_index] = x1;
		left_lines_y[left_index] = y1;
		left_index++;
		left_lines_x[left_index] = x2;
		left_lines_y[left_index] = y2;
		left_index++;
	}
	if (left_index > 0) {
		double c0, c1, cov00, cov01, cov11, sumsq;
		gsl_fit_linear(left_lines_x, 1, left_lines_y, 1, left_index,
			&c0, &c1, &cov00, &cov01, &cov11, &sumsq);
		left_m = c1;
		left_b = c0;
	}
	else {
		left_m = left_b = 1;
		draw_left = false;
	}
	//������ ���ʿ� ���� ���� �ΰ� ã��,draw����
	//y = m*x + b--> x = (y - b) / m
	int y1 = height;
	int y2 = height * (1 - trap_height);

	float right_x1 = (y1 - right_b) / right_m;
	float right_x2 = (y2 - right_b) / right_m;

	float left_x1 = (y1 - left_b) / left_m;
	float left_x2 = (y2 - left_b) / left_m;
	//float�� �Ȱ� int������ ��ȯ���ְ�
	right_x1 = int(right_x1);
	right_x2 = int(right_x2);
	left_x1 = int(left_x1);
	left_x2 = int(left_x2);
	y1 = int(y1);
	y2 = int(y2);
	//�̹����� ������ ���� ���� ���� �׷��ش�
	if (draw_right)
		line(img_line, Point(right_x1, y1), Point(right_x2, y2), Scalar(255, 0, 0), 10);
	if (draw_left)
		line(img_line, Point(left_x1, y1), Point(left_x2, y2), Scalar(255, 0, 0), 10);

}