#include "pch.h"
#include "Contours.h"

void Contours::setReferencedUID(const OFString &reUID)
{
	Contours::m_referencedUID = reUID;
}

const OFString &Contours::getReferencedUID()
{
	return m_referencedUID;
}

void Contours::setDataContour(const std::vector<cv::Point> &dataContour)
{
	Contours::m_dataContour = dataContour;
}

const std::vector<cv::Point> &Contours::getDataContour()
{
	return m_dataContour;
}

void Contours::addPoint(cv::Point point)
{
	m_dataContour.push_back(point);
}
void Contours::clearContour()
{
	m_referencedUID.clear();
	m_dataContour.clear();
}
