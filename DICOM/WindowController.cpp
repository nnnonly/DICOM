#include "pch.h"
#include "WindowController.h"
#include "AppDocument.h"



std::string WindowController::loadPath(CFileDialog& dlg)
{
	CString filePath = dlg.GetPathName();
	std::string sPath = filePath.GetString();
	return sPath;
}


std::vector<std::string> WindowController::split(std::string s, std::string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}
	res.push_back(s.substr(pos_start));
	return res;
}

int WindowController::loadInforImage(std::string path, cv::Mat &imag, Image &transImage)
{
	DcmFileFormat file_format;
	DcmDataset dataSet;
	OFCondition status = file_format.loadFile(path.c_str());
	dataSet = DcmDataset(*file_format.getDataset());
	AppDocument* saveData = AppDocument::getInstance();
	DicomImage *image = new DicomImage(path.c_str());
	image->setWindow(1048, 4096);

	if (image != NULL)
	{
		if (image->getStatus() == EIS_Normal)
		{

			OFString patientName;
			OFString patientID;
			OFString patientBirthDate;
			OFString patientSex;
			OFString patientAge;
			OFString patientWeight;
			OFString pixelSpacing;
			OFString rowPixelSpacing;
			OFString colPixelSpacing;
			OFString windowWidth;
			OFString windowCenter;
			OFString SOPInstanceUID;
			Float64 sliceThickness;
			Float64 sliceLocation;

			dataSet.findAndGetOFString(DCM_PatientName, patientName);
			dataSet.findAndGetOFString(DCM_PatientID, patientID);
			dataSet.findAndGetOFString(DCM_PatientBirthDate, patientBirthDate);
			dataSet.findAndGetOFString(DCM_PatientSex, patientSex);
			dataSet.findAndGetOFString(DCM_PatientAge, patientAge);
			dataSet.findAndGetOFString(DCM_PatientWeight, patientWeight);
			dataSet.findAndGetOFStringArray(DCM_PixelSpacing, pixelSpacing);
			dataSet.findAndGetOFString(DCM_WindowWidth, windowWidth);
			dataSet.findAndGetOFString(DCM_WindowCenter, windowCenter);
			dataSet.findAndGetOFString(DCM_SOPInstanceUID, SOPInstanceUID);
			dataSet.findAndGetFloat64(DCM_SliceThickness, sliceThickness);
			dataSet.findAndGetFloat64(DCM_SliceLocation, sliceLocation);

			cv::Mat img(int(image->getWidth()), int(image->getHeight()), CV_8U, (uchar*)image->getOutputData(8));

			transImage.setPatientName(patientName.c_str());
			transImage.setPatientID(patientID.c_str());
			transImage.setPatientBirthDate(patientBirthDate.c_str());
			transImage.setPatientSex(patientSex.c_str());
			transImage.setPatientAge(patientAge.c_str());
			transImage.setPatientWeight(patientWeight.c_str());

			transImage.setWidth(windowWidth.c_str());
			transImage.setCenter(windowCenter.c_str());
			transImage.setSOPInstanceUID(SOPInstanceUID);
			transImage.setSliceThickness(sliceThickness);
			transImage.setSliceLocation(sliceLocation);

			std::string s = pixelSpacing.c_str();
			std::string delim = "\\";

			std::vector<std::string> v = split(s, delim);

			transImage.setColumnPixelSpacing(std::stof(v[0]));
			transImage.setRowPixelSpacing(std::stof(v[1]));
			//int test = sliceThickness / std::stof(v[1]);
			transImage.setResize(sliceThickness / std::stof(v[1]));
			imag = img.clone();
			transImage.setMatImage(imag);
			saveData->setImage(transImage);
		}
		else
		{
			std::cerr << "Cannot load image";
			return 100;
		}
	}
	delete image;
	return 0;
}

void WindowController::loadPixel(int windowWidth, int windowCenter, const cv::Mat&img, cv::Mat& outputImage)
{
	float width = windowWidth * 255 / 4095.0;
	float center = (windowCenter + 1000) * 255 / 4095.0;
	int low = round(center - width / 2);
	int high = round(center + width / 2);
	if (low < 0)
	{
		low = 0;
	}
	if (low > 255)
	{
		low = 255;
	}
	if (high < 0)
	{
		high = 0;
	}
	if (high > 255)
	{
		high = 255;
	}

	outputImage = cv::Mat::zeros(img.rows, img.cols, img.type());

	uchar* imgData = img.data;
	uchar* outData = outputImage.data;

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			if (imgData[i * img.rows + j] >= high)
			{
				outData[i * img.rows + j] = 255;
			}
			if (imgData[i * img.rows + j] <= low)
			{
				outData[i * img.rows + j] = 0;
			}
			if (imgData[i * img.rows + j] > low && imgData[i * img.rows + j] < high)
			{
				int p = (imgData[i * img.rows + j] - low) * 255 / width;
				if (p > 255)
				{
					outData[i * img.rows + j] = 255;
				}
				else if (p < 0)
				{
					outData[i * img.rows + j] = 0;
				}
				else 
				{
					outData[i * img.rows + j] = p;
				}
			}
		}
	}
}

int WindowController::listRoiName(DcmFileFormat &fileformat, std::vector<OFString> &listRName)
{
	signed long i = 0;
	DcmItem *roiitem = NULL;
	DcmItem *contouritem = NULL;
	DcmItem *contourItemSequence = NULL;
	std::vector<OFString> listName;
	while (fileformat.getDataset()->findAndGetSequenceItem(DCM_RTROIObservationsSequence, roiitem, i++).good())
	{
		OFString name;
		OFString des;
		unsigned long j = 0;
		unsigned long k = 0;
		while (roiitem->findAndGetOFString(DCM_ROIObservationLabel, name, j++).good())
		{
			listName.push_back(name);
		}
		while (roiitem->findAndGetOFString(DCM_RTROIInterpretedType, name, k++).good())
		{
			if (std::string(name.c_str()) == "MARKER")
			{
				roiitem->findAndGetOFString(DCM_ROIObservationDescription, des);
				listName.push_back(des);
			}
		}
	}
	listRName = listName;
	listName.clear();
	return 0;
}

int WindowController::readRTStruct(DcmFileFormat &fileformat, std::string filePath, std::vector<Volume> &listVol, std::vector<OFString> &listROIName)
{
	DcmItem *roiitem = NULL;
	DcmItem *contouritem = NULL;
	DcmItem *contourItemSequence = NULL;
	signed long ri = 0;
	int idx = 0;
	Volume mVolume;
	Contours mContour;

	while (fileformat.getDataset()->findAndGetSequenceItem(DCM_ROIContourSequence, roiitem, ri++).good())
	{
		// roiitem now points to one item in the ROIContourSequence
		signed long cl = 0;
		OFString color;
		int arrColor[3];
		int idx = 0;
		while (roiitem->findAndGetOFString(DCM_ROIDisplayColor, color, cl++).good())
		{
			arrColor[idx] = (int)color.c_str();
			idx++;
		}
		mVolume.setRoiColor(cv::Scalar(arrColor[0], arrColor[1], arrColor[3]));
		signed long ci = 0;
		while (roiitem->findAndGetSequenceItem(DCM_ContourSequence, contouritem, ci++).good())
		{
			unsigned long cs = 0;
			while (contouritem->findAndGetSequenceItem(DCM_ContourImageSequence, contourItemSequence, cs++).good())
			{
				OFString uid;
				if (contourItemSequence->findAndGetOFStringArray(DCM_ReferencedSOPInstanceUID, uid).good())
				{
					std::vector<Float64> points;
					Float64 val;
					
					unsigned long cd = 0;
					while (contouritem->findAndGetFloat64(DCM_ContourData, val, cd++).good())
					{
						points.push_back(val);
					}
					mContour.setReferencedUID(uid);
					for (int idx = 0; idx < points.size(); idx+=3)
					{
						cv::Point point;
						point.x = points[idx] * 0;
						point.y = points[idx + 1] * 2;
						//point.z = points[idx + 2];
						mContour.addPoint(point);
					}
					mVolume.addContours(mContour);
					mVolume.setRoiName(listROIName[idx].c_str());
				}
			}
		}
		idx++;
	}
	return 0;
}

std::vector<std::vector<cv::Point>> WindowController::genarateExternal(cv::Mat &inputImg)
{
	cv::Mat thresholdedImage;
	cv::threshold(inputImg, thresholdedImage, 52, 255, 0);

	cv::Mat kernel_02 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5), cv::Point(3, 3));
	cv::morphologyEx(thresholdedImage, thresholdedImage, cv::MORPH_OPEN, kernel_02);

	//To store contours q
	std::vector<std::vector<cv::Point>> contours;

	//To store hierarchy(nestedness)
	std::vector<cv::Vec4i> hierarchy;

	//Find contours
	findContours(thresholdedImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	//Output image to draw contours on
	//cv::Mat output = cv::Mat::zeros(thresholdedImage.size(), CV_8UC3);

	//cv::RNG rng(12345);

	// Draw contours.
	//for (size_t i = 0; i < contours.size(); i++) {
	//	cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	//	drawContours(output, contours, i, color, 2);
	//}

	return contours;
}

