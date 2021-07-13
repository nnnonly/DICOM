#include "pch.h"
#include "Image.h"

void Image::setMatImage(cv::Mat &inImage)
{
	image = inImage;
}

const cv::Mat &Image::getMatImage()
{
	return image;
}

void Image::setPatientName(const std::string &patientName)
{
	Image::patientName = patientName;
}

const std::string &Image::getPatientName()
{
	return patientName;
}

void Image::setPatientID(const std::string &patientID)
{
	Image::patientID = patientID;
}

const std::string &Image::getPatientID()
{
	return patientID;
}

void Image::setPatientBirthDate(const std::string &birthDate)
{
	Image::patientBirthDate = birthDate;
}

const std::string &Image::getPatientBirthDate()
{
	return patientBirthDate;
}

void Image::setPatientAge(const std::string &patientAge)
{
	Image::patientAge = patientAge;
}

const std::string &Image::getPatientAge()
{
	return patientAge;
}

void Image::setPatientSex(const std::string &patientSex)
{
	Image::patientSex = patientSex;
}

const std::string &Image::getPatientSex()
{
	return patientSex;
}

void Image::setPatientWeight(const std::string &patientWeight)
{
	Image::patientWeight = patientWeight;
}

const std::string &Image::getPatientWeight()
{
	return patientWeight;
}

void Image::setWidth(const std::string &windowWidth)
{
	Image::windowWidth = windowWidth;
}

const std::string &Image::getWidth()
{
	return windowWidth;
}

void Image::setCenter(const std::string &windowCenter)
{
	Image::windowCenter = windowCenter;
}

const std::string &Image::getCenter()
{
	return windowCenter;
}

void Image::setPixelSpacing(const std::string &pixelSpacing)
{
	Image::pixelSpacing = pixelSpacing;
}

const std::string &Image::getPixelSpacing()
{
	return pixelSpacing;
}

void Image::setSliceThickness(const int &sliceThickness)
{
	Image::sliceThickness = sliceThickness;
}

const int &Image::getSliceThickness()
{
	return sliceThickness;
}

void Image::setSliceLocation(const float &sliceLocation)
{
	Image::sliceLocation = sliceLocation;
}

const float &Image::getSliceLocation()
{
	return sliceLocation;
}
void Image::setRowPixelSpacing(const float &row_pixel_spacing)
{
	Image::rowPixelSpacing = row_pixel_spacing;
}

const float &Image::getRowPixelSpacing()
{
	return rowPixelSpacing;
}

void Image::setColumnPixelSpacing(const float &column_pixel_spacing)
{
	Image::colPixelSpacing = column_pixel_spacing;
}

const float &Image::getColumnPixelSpacing()
{
	return colPixelSpacing;
}

void Image::setResize(const float &resize)
{
	Image::autoResize = resize;
}

const float &Image::getResize()
{
	return autoResize;
}

void Image::setLocation(const int &location)
{
	Image::location = location;
}

const int &Image::getLocation()
{
	return location;
}

void Image::setSOPInstanceUID(OFString &SOPInstanceUID)
{
	Image::SOPInstanceUID = SOPInstanceUID;
}

OFString &Image::getSOPInstanceUID()
{
	return SOPInstanceUID;
}