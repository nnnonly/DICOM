#pragma once
#include "pch.h"

class Image
{
private:
	cv::Mat image;
	std::string patientName;
	std::string patientID;
	std::string patientBirthDate;
	std::string patientAge;
	std::string patientSex;
	std::string patientWeight;
	std::string windowWidth;
	std::string windowCenter;
	std::string pixelSpacing;
	float rowPixelSpacing;
	float colPixelSpacing;
	float sliceLocation;
	float autoResize;
	int location;
	int sliceThickness;
	OFString SOPInstanceUID;

public:
	void setMatImage(cv::Mat &inImage);

	const cv::Mat &getMatImage();

	void setPatientName(const std::string &patient_name);

	const std::string &getPatientName();

	void setPatientID(const std::string &patientID);

	const std::string &getPatientID();

	void setPatientBirthDate(const std::string &patientID);

	const std::string &getPatientBirthDate();

	void setPatientAge(const std::string &patientAge);

	const std::string &getPatientAge();

	void setPatientSex(const std::string &patientSex);

	const std::string &getPatientSex();

	void setPatientWeight(const std::string &patientWeight);

	const std::string &getPatientWeight();

	void setWidth(const std::string &windowWidth);

	const std::string &getWidth();

	void setCenter(const std::string &windowCenter);

	const std::string &getCenter();

	void setPixelSpacing(const std::string &pixelSpacing);

	const std::string &getPixelSpacing();

	void setSliceThickness(const int &sliceThickness);

	const int &getSliceThickness();

	void setSliceLocation(const float &sliceLocation);

	const float &getSliceLocation();

	void setRowPixelSpacing(const float &row_pixel_spacing);

	const float &getRowPixelSpacing();

	void setColumnPixelSpacing(const float &column_pixel_spacing);

	const float &getColumnPixelSpacing();

	void setResize(const float &resize);

	const float &getResize();

	void setLocation(const int &location);

	const int &getLocation();

	void setSOPInstanceUID(OFString &SOPInstanceUID);

	OFString &getSOPInstanceUID();
};
