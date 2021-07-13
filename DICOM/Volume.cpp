#include "pch.h"
#include "Volume.h"

void Volume::setRoiNumber(const int &roiNumber)
{
	Volume::roiNumber = roiNumber;
}

int &Volume::getRoiNumber()
{
	return roiNumber;
}

void Volume::setRoiName(const std::string &roiName)
{
	Volume::roiName = roiName;
}

std::string &Volume::getRoiName()
{
	return roiName;
}

void Volume::setRoiColor(cv::Scalar &roiColor)
{
	Volume::roiColor = roiColor;
}

cv::Scalar &Volume::getRoiColor()
{
	return roiColor;
}

void Volume::setListContour(std::vector<Contours> &listContour)
{
	Volume::listContours = listContour;
}

std::vector<Contours> &Volume::getListContours()
{
	return listContours;
}

void Volume::addContours(Contours &contours)
{
	listContours.push_back(contours);
}

void Volume::clearVolume()
{
	listContours.clear();
}

std::vector<Contours> Volume::getContourSlice(Slice &slice)
{
	std::vector<Contours> contourVol;
	for (int idxSlice = 0; idxSlice < slice.getListContour().size(); idxSlice++)
	{
		for (int idxVolume = 0; idxVolume < this->getListContours().size(); idxVolume++)
		{
			if (slice.getListContour()[idxSlice]->getReferencedUID() == this->getListContours()[idxVolume].getReferencedUID())
			{
				auto contour1 = slice.getListContour()[idxSlice];
				auto contour2 = &this->getListContours()[idxVolume];
				//contourVol.push_back(*slice.getListContour()[idxSlice]);
				contourVol.push_back(this->getListContours()[idxVolume]);
			}
		}
	}
	return contourVol;
}
