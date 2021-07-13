#include "pch.h"
#include "DetectMarker.h"

void DetectMarker::GeneratePlusMask(cv::Mat& maskPlus, double rowPS, double colPS, int height, int width)
{
	int kerRows = width / rowPS;
	int kerCols = height / colPS;

	cv::Mat kernelPlus = cv::Mat::zeros(kerRows, kerCols, CV_32F);

	cv::line(kernelPlus, cv::Point(kerRows / 2, 0), cv::Point(kerRows / 2, kerRows - 1), 1, 4);
	cv::line(kernelPlus, cv::Point(0, kerCols / 2), cv::Point(kerCols - 1, kerCols / 2), 1, 4);
	cv::circle(kernelPlus, cv::Point(kerRows / 2, kerCols / 2), 2, 0, 4);

	maskPlus = kernelPlus;
}

void DetectMarker::GenerateMultiMask(cv::Mat& maskMulti, double rowPS, double colPS, int height, int width)
{
	int kerRows = width / rowPS;
	int kerCols = height / colPS;

	cv::Mat kernelMulti = cv::Mat::zeros(kerRows, kerCols, CV_32F);
	cv::line(kernelMulti, cv::Point(0, 0), cv::Point(kerRows - 1, kerCols - 1), 1, 4);
	cv::line(kernelMulti, cv::Point(kerRows - 1, 0), cv::Point(0, kerCols - 1), 1, 4);

	cv::circle(kernelMulti, cv::Point(kerRows / 2, kerCols / 2), 2, 0, 4);
	maskMulti = kernelMulti;
}