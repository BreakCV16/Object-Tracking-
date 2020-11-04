#ifndef Converter_h
#define Converter_h
#include <opencv2/opencv.hpp>
class Converter {
public:
	void RGBtoHSI(uchar r, uchar g, uchar b, double HSI[]);

	void HSItoRGB(double h, double s, double i, unsigned char** rgb);

	void RGBtoHsv(uchar r, uchar g, uchar b, double Hsv[]);

	void HsvToRGB(double h, double s, double v, unsigned char** rgb);
};


#endif
