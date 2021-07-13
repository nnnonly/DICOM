#pragma once
#include "pch.h"
#include "Contours.h"
#include "Slice.h"

class Volume
{
private:
	int roiNumber;
	std::string roiName;
	cv::Scalar roiColor;
	std::vector<Contours> listContours;

public:
	void setRoiNumber(const int &roiNumber);

	int &getRoiNumber();

	void setRoiName(const std::string &roiName);

	std::string &getRoiName();

	void setRoiColor(cv::Scalar &roiColor);

	cv::Scalar &getRoiColor();

	void setListContour(std::vector<Contours> &listContour);

	std::vector<Contours> &getListContours();

	void addContours(Contours &contours);

	void clearVolume();

	std::vector<Contours> getContourSlice(Slice &slice);
};

