#include "pch.h"
#include "DetectMarker.h"

const int thresholdMin = 70;
const int thresholdMax = 150;

const float minPlus = 0.85;
const float minMulti = 0.7;
const float max = 1.0;

void DetectMarker::preProcessing(const cv::Mat &img)
{
	cv::medianBlur(img, img, 3);

	cv::GaussianBlur(img, img, cv::Size(3, 3), 0);

	//showImageDebug(img, "Noise Reduce");

	cv::Canny(img, img, thresholdMin, thresholdMax);

	//showImageDebug(img, "Canny");

	cv::Mat kernel_02 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3), cv::Point(1, 1));
	cv::morphologyEx(img, img, cv::MORPH_OPEN, kernel_02);

	cv::Mat kernel_01 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5), cv::Point(2, 2));
	cv::morphologyEx(img, img, cv::MORPH_CLOSE, kernel_01);


}

void DetectMarker::featureExtraction(const cv::Mat& img, cv::Mat& maskPlus, cv::Mat& maskMulti, std::vector<cv::Point2f> &centerPointPlus, std::vector<cv::Point2f> &centerPointMulti)
{
	cv::Mat result = img.clone();
	//cv::cvtColor(result, result, CV_GRAY2BGR);

	cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, CV_32F);

	cv::Mat convPlus;
	cv::filter2D(result, convPlus, -1, maskPlus);

	cv::normalize(convPlus, convPlus, 0, 1, cv::NORM_MINMAX, CV_32F);

	//showImageDebug(convPlus, "Convolution");

	cv::inRange(convPlus, minPlus, max, convPlus);

	//showImageDebug(convPlus, "Threshold");

	std::vector<std::vector<cv::Point> > contoursPlus;
	std::vector<cv::Vec4i> hierarchyPlus;
	cv::findContours(convPlus, contoursPlus, hierarchyPlus, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	for (int i = 0; i < contoursPlus.size(); i++)
	{
		auto muPlus = cv::moments(contoursPlus[i], false);
		centerPointPlus.push_back(cv::Point2f(muPlus.m10 / muPlus.m00, muPlus.m01 / muPlus.m00));
	}

	cv::Mat convMulti;
	cv::filter2D(result, convMulti, -1, maskMulti);

	cv::normalize(convMulti, convMulti, 0, 1, cv::NORM_MINMAX, CV_32F);
	cv::inRange(convMulti, minMulti, max, convMulti);

	std::vector<std::vector<cv::Point> > contoursMulti;

	cv::findContours(convMulti, contoursMulti, hierarchyPlus, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	for (int i = 0; i < contoursMulti.size(); i++)
	{
		auto muMulti = cv::moments(contoursMulti[i], false);
		centerPointMulti.push_back(cv::Point2f(muMulti.m10 / muMulti.m00, muMulti.m01 / muMulti.m00));
	}
}