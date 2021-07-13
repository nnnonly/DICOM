#pragma once

#include "pch.h"
#include "Image.h"
#include "Contours.h"
#include "Slice.h"
#include "Volume.h"
#include "WindowController.h"

class AppDocument
{
	public:
		static AppDocument* getInstance();

		void setDicomPath(std::string &dicomPath);

		const std::string &getDicomPath();

		void setImage(const Image &image);

		Image &getImage();

		void setSagittalImage(cv::Mat &sagittalImage);

		const cv::Mat &getSagittalImage();

		void setCoronalImage(cv::Mat &coronalImage);

		const cv::Mat &getCoronalImage();

		void setTransverseImage(const std::vector<Image> &transverseImage);

		std::vector<Image> &getTransverseImage();

		void addTransImage(Image &inImage);

		void setImgData(const std::vector<cv::Mat> &imageData);

		const std::vector<cv::Mat> &getImgData();

		void setFolSagittalImage(const std::vector<cv::Mat> &folSagittalImage);

		const std::vector<cv::Mat> &getFolSagittalImage();

		void setFolCoronalImage(const std::vector<cv::Mat> &folSagittalImage);

		const std::vector<cv::Mat> &getFolCoronalImage();

		void setPointPlus(std::vector<cv::Point2f> &pointPlus);

		std::vector<cv::Point2f> &getPointPlus();

		void setPointMulti(std::vector<cv::Point2f> &pointMulti);

		std::vector<cv::Point2f> &getPointMulti();

		void setListContours(std::string path, float pix, int bonusX, int bonusY);

		std::vector<Contours> &getListContour();

		void setListVolume(std::string path, float pix, int bonusX, int bonusY);

		std::vector<Volume> &getListVolume();

		void setListMap(std::vector<Image> &img, std::vector<Contours> &cons);

		void setList(std::vector<Image> &listImg);

		std::vector<Slice> &getListMap();

		std::vector<Contours> getContourSlice(Slice &slice, Volume &volume);

		void setListExternal(std::vector<Image> &listImg);

		std::vector<Slice> &getListExternal();

		std::vector<std::vector<std::vector<cv::Point>>> &getlistConExt();

	private:
		static AppDocument* instance;

		Image image;
		std::string dicomPath;
		cv::Mat sagittalImage;
		cv::Mat coronalImage;

		std::vector<cv::Mat> imageData;
		std::vector<Image> transverseImage;
		std::vector<cv::Mat> folSagittalImage;
		std::vector<cv::Mat> folCoronalImage;
		std::vector<cv::Point2f> centerPointPlus;
		std::vector<cv::Point2f> centerPointMulti;

		std::vector<Contours> listContour;
		std::vector<Volume> listVolume;
		std::vector<Slice> listMap;
		std::vector<Slice> listExternal;
		std::vector<std::vector<std::vector<cv::Point>>> listConExt;
};