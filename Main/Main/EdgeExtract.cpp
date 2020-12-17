#include <iostream>
#include <opencv2/opencv.hpp>
#define PI 3.14159265359
//#include <gsl/gsl_fit.h>

using namespace cv;
using namespace std;

//*** 차선 색깔 범위 ***// 
//흰색 차선 (RGB)
Scalar white_Max = Scalar(255, 255, 255);
Scalar white_min = Scalar(200, 200, 200);
// 노란색 차선(HSV)
Scalar yellow_Max = Scalar(40, 255, 255);
Scalar yellow_min = Scalar(10, 100, 100);

//*** 영상의 흰색과 노란색 부분만 남기는 과정 ***//
Mat extract_colors(Mat Ori)
{
	Mat img_bgr, img_hsv, img_mixed;
	Mat white_mask, white_image, yellow_mask, yellow_image;
	Ori.copyTo(img_bgr);

	//흰색 추출
	inRange(img_bgr, white_min, white_Max, white_mask);
	bitwise_and(img_bgr, img_bgr, white_image, white_mask);

	//노란색 추출
	cvtColor(img_bgr, img_hsv, COLOR_BGR2HSV);

	inRange(img_hsv, yellow_min, yellow_Max, yellow_mask);
	bitwise_and(img_bgr, img_bgr, yellow_image, yellow_mask);

	//두 색의 결과를 합치는 과정
	addWeighted(white_image, 1.0, yellow_image, 1.0, 0.0, img_mixed);

	return img_mixed;
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

void GaussianFilter(Mat In, Mat Out, int nWidth, int nHeight, double sigma)
{
	double gaussian[3][3] = { 0.0 };
	double sum = 0.0;

	for (int h = 0; h < 3; h++)
	{
		for (int w = 0; w < 3; w++)
		{
			double num = -(double)(h * h + w * w) / ((2 * sigma * sigma));
			double e = exp(num);
			double den = 2 * PI * sigma * sigma;
			gaussian[h][w] = (1.0 / den) * e;
			sum += gaussian[h][w];
		}
	}

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			double result = 0;
			int i = 0;
			for (int th = h; th < 3 + h; th++)
			{
				int j = 0;
				for (int tw = w; tw < 3 + w; tw++)
				{
					result += In.at<uchar>(th, tw) * (gaussian[i][j++] / sum);
				}
				i++;
			}
			if (result > 255)
				result = 255;
			else if (result < 0)
				result = 0;

			Out.at<uchar>(h, w) = (unsigned char)result;
		}
	}
}

void Apply_Cannyf(Mat img_filtered, Mat& img_edges) {

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

		//-3.GaussianFilter

	Mat img_blur = Mat::zeros(img_pad.rows, img_pad.cols, 0);
	GaussianFilter(img_blur, img_blur, img_gray.cols, img_gray.rows, 0);

	//GaussianBlur(img_pad, img_blur, Size(3, 3), 0, 0);
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