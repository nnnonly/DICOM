#include "pch.h"
#include "DetectMarker.h"

void DetectMarker::drawMarkerPlus(const cv::Mat &outputImg, std::vector<cv::Point2f> &Point)
{
	for (int i = 0; i < Point.size(); i++)
	{
		cv::circle(outputImg, Point[i], 7, cv::Scalar(0, 0, 255), CV_FILLED);
		cv::circle(outputImg, Point[i], 18, cv::Scalar(0, 0, 255), 2);
		//cv::circle(drawing, mc[i], 4, color, -1, 8, 0);
	}

	for (int i = 1; i < Point.size() - 1; i++)
	{
		cv::line(outputImg, Point[i], Point[i + 1], cv::Scalar(0, 255, 0), 2);
	}
	cv::line(outputImg, Point[Point.size() - 1], Point[1], cv::Scalar(0, 255, 0), 2);
}

void DetectMarker::drawMarkerMulti(const cv::Mat &outputImg, std::vector<cv::Point2f> &Point)
{
	for (int i = 0; i < Point.size(); i++)
	{
		cv::circle(outputImg, Point[i], 7, cv::Scalar(255, 0, 0), CV_FILLED);
		cv::circle(outputImg, Point[i], 18, cv::Scalar(255, 0, 0), 2);
		//cv::circle(drawing, mc[i], 4, color, -1, 8, 0);
	}

	for (int i = 0; i < Point.size(); i++)
	{
		for (int j = i + 1; j < Point.size(); j++)
		{
			if (abs(Point[i].x - Point[j].x) <= 30 || abs(Point[i].y - Point[j].y) <= 30)
			{
				cv::line(outputImg, Point[i], Point[j], cv::Scalar(0, 255, 0), 2);
			}
		}
	}
}