#include "pch.h"
#include "DetectMarker.h"
#define PI 3.14159265

const int epsilon = 3;

std::vector<cv::Point2f> DetectMarker::postProcessingPlus(const cv::Mat& imgIn, std::vector<cv::Point2f> &centerPoint)
{
	std::vector<cv::Point2f> centerCoppy;
	std::vector<cv::Point2f> ouputPoint;
	//cv::cvtColor(imgOut, imgOut, CV_GRAY2BGR);

	//centerPoint.reserve(centerPoint.size());

	for (int i = 0; i < centerPoint.size(); i++)
	{
		centerCoppy.push_back(centerPoint[i]);
	}
	centerPoint.push_back(centerPoint[0]);

	if (centerPoint.size() >= 3)
	{
		for (int i = 0; i < centerPoint.size() - 2; i++)
		{
			float vector1x = centerPoint[i].x - centerPoint[i + 1].x;
			float vector1y = centerPoint[i].y - centerPoint[i + 1].y;
			float vector2x = centerPoint[i + 1].x - centerPoint[i + 2].x;
			float vector2y = centerPoint[i + 1].y - centerPoint[i + 2].y;
			float length1 = sqrt(vector1x * vector1x + vector1y * vector1y);
			float length2 = sqrt(vector2x * vector2x + vector2y * vector2y);
			float length3 = sqrt((centerPoint[i].x - centerPoint[i + 2].x)*(centerPoint[i].x - centerPoint[i + 2].x) + (centerPoint[i].y - centerPoint[i + 2].y)*(centerPoint[i].y - centerPoint[i + 2].y));

			double cos = (abs(vector1x * vector2x + vector1y * vector2y)) / (length1*length2);
			if (abs(acos(cos) * 180 / PI) <= epsilon)
			{
				if (abs(length1 + length2 - length3) <= epsilon)
				{
					ouputPoint.push_back(centerCoppy[i + 1]);
					centerCoppy.erase(centerCoppy.begin() + i + 1);
				}
				if (abs(length1 + length3 - length2) <= epsilon)
				{
					ouputPoint.push_back(centerCoppy[i + 2]);
					centerCoppy.erase(centerCoppy.begin() + i + 2);
				}
				if (abs(length2 + length3 - length1) <= epsilon)
				{
					ouputPoint.push_back(centerCoppy[i]);
					centerCoppy.erase(centerCoppy.begin() + i);
				}
			}
		}
	}

	for (int i = 0; i < centerCoppy.size(); i++)
	{
		ouputPoint.push_back(centerCoppy[i]);
	}

	for (int i = 0; i < centerCoppy.size() - 2; i++)
	{
		float vector1x = centerCoppy[i].x - centerCoppy[i + 1].x;
		float vector1y = centerCoppy[i].y - centerCoppy[i + 1].y;
		float vector2x = centerCoppy[i + 1].x - centerCoppy[i + 2].x;
		float vector2y = centerCoppy[i + 1].y - centerCoppy[i + 2].y;
		float vector3x = centerCoppy[i].x - centerCoppy[i + 2].x;
		float vector3y = centerCoppy[i].y - centerCoppy[i + 2].y;
		float length1 = sqrt(vector1x * vector1x + vector1y * vector1y);
		float length2 = sqrt(vector2x * vector2x + vector2y * vector2y);
		float length3 = sqrt((centerCoppy[i].x - centerCoppy[i + 2].x)*(centerCoppy[i].x - centerCoppy[i + 2].x) + (centerCoppy[i].y - centerCoppy[i + 2].y)*(centerCoppy[i].y - centerCoppy[i + 2].y));

		float cos12 = (abs(vector1x * vector2x + vector1y * vector2y)) / (length1 * length2);
		float cos23 = (abs(vector3x * vector2x + vector3y * vector2y)) / (length3 * length2);
		float cos13 = (abs(vector1x * vector3x + vector1y * vector3y)) / (length1 * length3);
		if (abs(acos(cos12) * 180 / PI - 90) <= epsilon)
		{
			cv::Point2f findPoint = cv::Point2f((centerCoppy[i].x + centerCoppy[i + 2].x - centerCoppy[i + 1].x), (centerCoppy[i].y + centerCoppy[i + 2].y - centerCoppy[i + 1].y));
			ouputPoint.push_back(findPoint);
		}
		if (abs(acos(cos23) * 180 / PI - 90) <= epsilon)
		{
			cv::Point2f findPoint = cv::Point2f((centerCoppy[i].x + centerCoppy[i + 1].x - centerCoppy[i + 2].x), (centerCoppy[i].y + centerCoppy[i + 1].y - centerCoppy[i + 2].y));
			ouputPoint.push_back(findPoint);
		}
		if (abs(acos(cos13) * 180 / PI - 90) <= epsilon)
		{
			cv::Point2f findPoint = cv::Point2f((centerCoppy[i + 1].x + centerCoppy[i + 2].x - centerCoppy[i].x), (centerCoppy[i + 1].y + centerCoppy[i + 2].y - centerCoppy[i].y));
			ouputPoint.push_back(findPoint);
		}

	}
	return ouputPoint;
}

std::vector<cv::Point2f> DetectMarker::postProcessingMulti(const cv::Mat& imgIn, std::vector<cv::Point2f> &centerPoint)
{
	if (centerPoint.size() == 4)
	{
		return centerPoint;
	}
	else
	{
		std::vector<cv::Point2f> ouputPoint;
		for (int i = 0; i < centerPoint.size(); i++)
		{
			ouputPoint.push_back(centerPoint[i]);
		}
		for (int i = 0; i < centerPoint.size() - 2; i++)
		{
			float vector1x = centerPoint[i].x - centerPoint[i + 1].x;
			float vector1y = centerPoint[i].y - centerPoint[i + 1].y;
			float vector2x = centerPoint[i + 1].x - centerPoint[i + 2].x;
			float vector2y = centerPoint[i + 1].y - centerPoint[i + 2].y;
			float vector3x = centerPoint[i].x - centerPoint[i + 2].x;
			float vector3y = centerPoint[i].y - centerPoint[i + 2].y;
			float length1 = sqrt(vector1x * vector1x + vector1y * vector1y);
			float length2 = sqrt(vector2x * vector2x + vector2y * vector2y);
			float length3 = sqrt((centerPoint[i].x - centerPoint[i + 2].x)*(centerPoint[i].x - centerPoint[i + 2].x) + (centerPoint[i].y - centerPoint[i + 2].y)*(centerPoint[i].y - centerPoint[i + 2].y));

			float cos12 = (abs(vector1x * vector2x + vector1y * vector2y)) / (length1 * length2);
			float cos23 = (abs(vector3x * vector2x + vector3y * vector2y)) / (length3 * length2);
			float cos13 = (abs(vector1x * vector3x + vector1y * vector3y)) / (length1 * length3);
			if (abs(acos(cos12) * 180 / PI - 90) <= epsilon)
			{
				cv::Point2f findPoint = cv::Point2f((centerPoint[i].x + centerPoint[i + 2].x - centerPoint[i + 1].x), (centerPoint[i].y + centerPoint[i + 2].y - centerPoint[i + 1].y));
				ouputPoint.push_back(findPoint);
			}
			if (abs(acos(cos23) * 180 / PI - 90) <= epsilon)
			{
				cv::Point2f findPoint = cv::Point2f((centerPoint[i].x + centerPoint[i + 1].x - centerPoint[i + 2].x), (centerPoint[i].y + centerPoint[i + 1].y - centerPoint[i + 2].y));
				ouputPoint.push_back(findPoint);
			}
			if (abs(acos(cos13) * 180 / PI - 90) <= epsilon)
			{
				cv::Point2f findPoint = cv::Point2f((centerPoint[i + 1].x + centerPoint[i + 2].x - centerPoint[i].x), (centerPoint[i + 1].y + centerPoint[i + 2].y - centerPoint[i].y));
				ouputPoint.push_back(findPoint);
			}
		}
		return ouputPoint;
	}
}