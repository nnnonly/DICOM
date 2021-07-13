#pragma once
#include "pch.h"
class Contours
{
	public:
		void setReferencedUID(const OFString &reUID);

		const OFString &getReferencedUID();

		void setDataContour(const std::vector<cv::Point> &dataContour);

		const std::vector<cv::Point> &getDataContour();

		void addPoint(cv::Point point);

		void clearContour();

	protected:
		cv::Point3f getPoint(std::vector<Float64> pointContour,  uint p);

	private:
		OFString m_referencedUID;
		std::vector<cv::Point> m_dataContour;
};

