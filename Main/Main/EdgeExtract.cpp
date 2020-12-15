#include <iostream>
#include <opencv2/opencv.hpp>
//#include <gsl/gsl_fit.h>

using namespace cv;
using namespace std;

//*** ���� ���� ���� ***// 
//��� ���� (RGB)
Scalar white_Max = Scalar(255, 255, 255);
Scalar white_min = Scalar(200, 200, 200);
// ����� ����(HSV)
Scalar yellow_Max = Scalar(40, 255, 255);
Scalar yellow_min = Scalar(10, 100, 100);

//*** ������ ����� ����� �κи� ����� ���� ***//
Mat extract_colors(Mat Ori)
{
	Mat img_bgr, img_hsv, img_mixed;
	Mat white_mask, white_image, yellow_mask, yellow_image;
	Ori.copyTo(img_bgr);

	//��� ����
	inRange(img_bgr, white_min, white_Max, white_mask);
	bitwise_and(img_bgr, img_bgr, white_image, white_mask);

	//����� ����
	cvtColor(img_bgr, img_hsv, COLOR_BGR2HSV);

	inRange(img_hsv, yellow_min, yellow_Max, yellow_mask);
	bitwise_and(img_bgr, img_bgr, yellow_image, yellow_mask);

	//�� ���� ����� ��ġ�� ����
	addWeighted(white_image, 1.0, yellow_image, 1.0, 0.0, img_mixed);

	return img_mixed;
}

//*** �������� �ν��� ���� ���� ***//
Mat region_of_interest(Mat img_edges, int height, int width)
{
	float trap_bottom_width = 0.85;  // ��ٸ����� �Ʒ��� �����ڸ� �ʺ�, �̹��� �ʺ��� ������� ǥ��
	float trap_top_width = 0.07;     // ��ٸ����� ��� �����ڸ� �ʺ�, �̹��� �ʺ��� ������� ǥ��
	float trap_height = 0.4;	// ��ٸ����� ����, �̹��� ������ ������� ǥ��

	Point points[4];
	points[0] = Point((width * (1 - trap_bottom_width)) / 2, height);
	points[1] = Point((width * (1 - trap_top_width)) / 2, height - height * trap_height);
	points[2] = Point(width - (width * (1 - trap_top_width)) / 2, height - height * trap_height);
	points[3] = Point(width - (width * (1 - trap_bottom_width)) / 2, height);

	Mat img_mask = Mat::zeros(img_edges.rows, img_edges.cols, CV_8UC1);

	Scalar ignore_mask_color = Scalar(255, 255, 255);
	const Point* ppt[1] = { points };
	int npt[] = { 4 };

	fillPoly(img_mask, ppt, npt, 1, Scalar(255, 255, 255), LINE_8); // ä���

	Mat img_masked;
	bitwise_and(img_edges, img_mask, img_masked);

	return img_masked;
}
Mat Filtering(unsigned char* input, int width, int height, int Fsize = 3)
{
	Mat Padding;
	unsigned char* pData;

	int Psize = Fsize / 2;
	Padding = Mat(height + 2 * Psize, width + 2 * Psize, CV_8UC1);
	pData = Padding.data;
	for (int i = 0; i < Padding.rows; i++)
	{
		for (int j = 0; j < Padding.cols; j++)
		{
			pData[i * Padding.cols + j] = 0;
		}
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			pData[(i + Psize) * Padding.cols + (j + Psize)] = input[i * width + j];
		}
	}
	return Padding;
}
Mat Sobel_Vertical(unsigned char* input, int width, int height, int Fsize = 3)
{
	Mat Filtered;
	unsigned char* fData;
	double** Kernel = new double* [Fsize];
	int Psize = int(Fsize / 2);

	for (int i = 0; i < Fsize; i++)
	{
		Kernel[i] = new double[Fsize];
	}

	for (int i = 0; i < Fsize; i++)
	{
		for (int j = 0; j < Fsize; j++)
		{
			Kernel[i][j] = -1;
		}
	}

	for (int i = 0; i < Fsize; i++)
	{
		for (int j = 0; j < Fsize; j++)
		{
			if (j == 1)
			{
				Kernel[i][j] = 0;
			}
			else if (j == 2)
			{
				Kernel[i][j] = -Kernel[i][j];
				if (i == 1)
				{
					Kernel[i][j] *= 2;
				}
			}
			else
			{
				if (i == 1)
				{
					Kernel[i][j] *= 2;
				}
			}
		}
	}
	Filtered = Mat(height, width, CV_8UC1);


	fData = Filtered.data;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			double Y = 0;
			for (int k = 0; k < Fsize; k++)
			{
				for (int l = 0; l < Fsize; l++)
				{
					Y += input[(i + k) * (width + 2 * Psize) + j + l] * Kernel[k][l];
				}
			}

			if (Y > 255)
			{
				fData[i * width + j] = 255;
			}
			else if (Y < 0)
			{
				fData[i * width + j] = 0;
			}
			else
			{
				fData[i * width + j] = Y;
			}
		}
	}


	for (int i = 0; i < Fsize; i++)
	{
		delete[] Kernel[i];
	}

	delete[] Kernel;

	return Filtered;
}
Mat Sobel_Horizontal(unsigned char* input, int width, int height, int Fsize = 3)
{
	Mat Filtered;
	unsigned char* fData;
	double** Kernel = new double* [Fsize];
	int Psize = int(Fsize / 2);

	for (int i = 0; i < Fsize; i++)
	{
		Kernel[i] = new double[Fsize];
	}

	for (int i = 0; i < Fsize; i++)
	{
		for (int j = 0; j < Fsize; j++)
		{
			Kernel[i][j] = -1;
		}
	}

	for (int i = 0; i < Fsize; i++)
	{
		for (int j = 0; j < Fsize; j++)
		{
			if (i == 1)
			{
				Kernel[i][j] = 0;
			}
			else if (i == 2)
			{
				Kernel[i][j] = -Kernel[i][j];
				if (j == 1)
				{
					Kernel[i][j] *= 2;
				}
			}
			else
			{
				if (j == 1)
				{
					Kernel[i][j] *= 2;
				}
			}
		}
	}

	Filtered = Mat(height, width, CV_8UC1);


	fData = Filtered.data;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			double Y = 0;
			for (int k = 0; k < Fsize; k++)
			{
				for (int l = 0; l < Fsize; l++)
				{
					Y += input[(i + k) * (width + 2 * Psize) + j + l] * Kernel[k][l];
				}
			}

			if (Y > 255)
			{
				fData[i * width + j] = 255;
			}
			else if (Y < 0)
			{
				fData[i * width + j] = 0;
			}
			else
			{
				fData[i * width + j] = Y;
			}
		}
	}


	for (int i = 0; i < Fsize; i++)
	{
		delete[] Kernel[i];
	}

	delete[] Kernel;

	return Filtered;
}
Mat NMS(unsigned char* input, int width, int height, int Fsize = 3)
{
	Mat Filtered;
	unsigned char* fData;
	double** Kernel = new double* [Fsize];
	int Psize = int(Fsize / 2);

	for (int i = 0; i < Fsize; i++)
	{
		Kernel[i] = new double[Fsize];
	}

	Filtered = Mat(height, width, CV_8UC1);


	fData = Filtered.data;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			double max;
			for (int k = 0; k < Fsize; k++)
			{
				for (int l = 0; l < Fsize; l++)
				{
					Kernel[k][l] = input[(i + k) * (width + 2 * Psize) + j + l];
				}
			}

			max = Kernel[0][0];

			for (int k = 0; k < Fsize; k++)
			{
				for (int l = 0; l < Fsize; l++)
				{
					if (Kernel[k][l] > max)
					{
						max = Kernel[k][l];
					}
				}
			}

			if (max == Kernel[Psize][Psize])
			{
				fData[i * width + j] = input[(i + Psize) * (width + 2 * Psize) + j + Psize];
			}
			else
			{
				fData[i * width + j] = 0;
			}
		}
	}


	for (int i = 0; i < Fsize; i++)
	{
		delete[] Kernel[i];
	}

	delete[] Kernel;

	return Filtered;
}
Mat DT(unsigned char* input, int width, int height, int Fsize = 3)
{
	Mat Filtered;
	unsigned char* fData;
	int Psize = int(Fsize / 2);

	Filtered = Mat(height, width, CV_8UC1);


	fData = Filtered.data;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (input[(i + Psize) * (width + 2 * Psize) + j + Psize] > 150)
			{
				fData[i * width + j] = 255;
			}
			else if (input[(i + Psize) * (width + 2 * Psize) + j + Psize] > 50)
			{
				fData[i * width + j] = 50;
			}
			else
			{
				fData[i * width + j] = 0;
			}

		}
	}

	return Filtered;
}
Mat hysteresis(unsigned char* input, int width, int height, int Fsize = 3)
{
	Mat Filtered;
	unsigned char* fData;
	double** Kernel = new double* [Fsize];
	int Psize = int(Fsize / 2);

	bool check = false;
	for (int i = 0; i < Fsize; i++)
	{
		Kernel[i] = new double[Fsize];
	}

	Filtered = Mat(height, width, CV_8UC1);


	fData = Filtered.data;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (input[(i + Psize) * (width + 2 * Psize) + j + Psize] == 255)
			{
				fData[i * width + j] = 255;
			}
			else if (input[(i + Psize) * (width + 2 * Psize) + j + Psize] == 0)
			{
				fData[i * width + j] = 0;
			}
			else
			{
				for (int k = 0; k < Fsize; k++)
				{
					for (int l = 0; l < Fsize; l++)
					{
						Kernel[k][l] = input[(i + k) * (width + 2 * Psize) + j + l];
					}
				}

				for (int k = 0; k < Fsize; k++)
				{
					for (int l = 0; l < Fsize; l++)
					{
						if (Kernel[k][l] == 255)
						{
							fData[i * width + j] = 255;
							check = true;
							break;
						}

					}
					if (check)
					{
						check = false;
						break;
					}
					else
					{
						fData[i * width + j] = 0;
						break;
					}
				}
			}

		}
	}
	for (int i = 0; i < Fsize; i++)
	{
		delete[] Kernel[i];
	}

	delete[] Kernel;

	return Filtered;

}

void Apply_Cannyf(Mat img_filtered, Mat &img_edges) {

	//-1. BGR2GRAY
	Mat img_gray = Mat(img_filtered.rows, img_filtered.cols, CV_8UC1);
	//cvtColor(img_filtered, img_edges, COLOR_BGR2GRAY);

	for (int i = 0; i < img_filtered.rows; i++)
	{
		for (int j = 0; j < img_filtered.cols; j++)
		{
			img_gray.at<uchar>(i, j) = img_filtered.at<Vec3b>(i, j)[0] * 0.114 + img_filtered.at<Vec3b>(i, j)[1] * 0.587 + img_filtered.at<Vec3b>(i, j)[2] * 0.299;
		}
	}
	//imshow("GRAY", img_gray);

		//-2.Padding
	Mat img_pad;
	img_pad = Filtering(img_gray.data, img_gray.cols, img_gray.rows);
	//imshow("padding", img_pad);

		//-3.GaussianFilter - �̿ϼ�
	Mat img_blur;
	GaussianBlur(img_pad, img_blur, Size(3, 3), 0, 0);
	//imshow("Gaussian Blur", img_blur);

		//-4.Sobel
	Mat img_sobelv;

	img_sobelv = Sobel_Vertical(img_pad.data, img_gray.cols, img_gray.rows);
	//imshow("sobel_vertical", img_sobelv);

	Mat img_sobelh;

	img_sobelh = Sobel_Horizontal(img_pad.data, img_gray.cols, img_gray.rows);
	//imshow("sobel_horizontal", img_sobelh);

	Mat img_sobel;

	bitwise_or(img_sobelh, img_sobelv, img_sobel);

	//imshow("sobel", img_sobel);

	//writer << img_sobel;

	Mat img_sobelp;

	img_sobelp = Filtering(img_sobel.data, img_gray.cols, img_gray.rows);

	//-5.Non-Maximum Suppression

	Mat img_nms;

	img_nms = NMS(img_sobelp.data, img_gray.cols, img_gray.rows);

	//writer << img_nms;
	//imshow("Non Maxium Suppression", img_nms);

		//-6.Double Thresholding
	Mat img_nmsp;

	img_nmsp = Filtering(img_nms.data, img_gray.cols, img_gray.rows);

	Mat img_dt;
	img_dt = DT(img_nmsp.data, img_gray.cols, img_gray.rows);
	//imshow("Double Threshold", img_dt);
	//writer << img_dt;

		//-7.Edge Detecting
	Mat img_dtp;

	img_dtp = Filtering(img_dt.data, img_gray.cols, img_gray.rows);
	Mat img_edge;
	img_edge = hysteresis(img_dtp.data, img_gray.cols, img_gray.rows);
	img_edge.copyTo(img_edges);
	
}