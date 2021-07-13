#pragma once

#include "AppDocument.h"
#include "Contours.h"
#include "Volume.h"
#include "Image.h"

namespace WindowController
{
	std::string loadPath(CFileDialog& dlg);
	std::vector<std::string> split(std::string s, std::string delimiter);
	int loadInforImage(std::string path, cv::Mat &imag, Image &transImage);
	void loadPixel(int windowWidth, int windowCenter, const cv::Mat&img, cv::Mat& outputImage);
	int readRTStruct(DcmFileFormat &fileformat, std::string filePath, std::vector<Volume> &listVol, std::vector<OFString> &listROIName);
	int listRoiName(DcmFileFormat &fileformat, std::vector<OFString> &istRName);
	std::vector<std::vector<cv::Point>> genarateExternal(cv::Mat &inputImg);
};

