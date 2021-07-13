#include "pch.h"
#include "AppDocument.h"

AppDocument *AppDocument::instance = NULL;

AppDocument* AppDocument::getInstance()
{
	if (instance == NULL)
	{
		instance = new AppDocument();
	}
	return instance;
}

void AppDocument::setDicomPath(std::string &dicomPath)
{
	AppDocument::dicomPath = dicomPath;
}

const std::string &AppDocument::getDicomPath()
{
	return dicomPath;
}

void AppDocument::setImage(const Image &image)
{
	AppDocument::image = image;
}

Image& AppDocument::getImage()
{
	return image;
}

void AppDocument::setSagittalImage(cv::Mat &sagittalImage)
{
	AppDocument::sagittalImage = sagittalImage;
}

const cv::Mat &AppDocument::getSagittalImage()
{
	return sagittalImage;
}

void AppDocument::setCoronalImage(cv::Mat &coronalImage)
{
	AppDocument::coronalImage = coronalImage;
}

const cv::Mat &AppDocument::getCoronalImage()
{
	return coronalImage;
}

void AppDocument::setImgData(const std::vector<cv::Mat> &imageData)
{
	AppDocument::imageData = imageData;
}

const std::vector<cv::Mat> &AppDocument::getImgData()
{
	return imageData;
}

void AppDocument::setTransverseImage(const std::vector<Image> &transverseImage)
{
	AppDocument::transverseImage = transverseImage;
}

void AppDocument::addTransImage(Image &inImage)
{
	transverseImage.push_back(inImage);
}

std::vector<Image> &AppDocument::getTransverseImage()
{
	return transverseImage;
}

void AppDocument::setFolSagittalImage(const std::vector<cv::Mat> &folSagittalImage)
{
	AppDocument::folSagittalImage = folSagittalImage;
}

const std::vector<cv::Mat> &AppDocument::getFolSagittalImage()
{
	return folSagittalImage;
}

void AppDocument::setFolCoronalImage(const std::vector<cv::Mat> &folCoronalImage)
{
	AppDocument::folCoronalImage = folCoronalImage;
}

const std::vector<cv::Mat> &AppDocument::getFolCoronalImage()
{
	return folCoronalImage;
}

void AppDocument::setPointPlus(std::vector<cv::Point2f> &pointPlus)
{
	AppDocument::centerPointPlus = pointPlus;
}

std::vector<cv::Point2f> &AppDocument::getPointPlus()
{
	return centerPointPlus;
}

void AppDocument::setPointMulti(std::vector<cv::Point2f> &pointMulti)
{
	AppDocument::centerPointMulti = pointMulti;
}

std::vector<cv::Point2f> &AppDocument::getPointMulti()
{
	return centerPointMulti;
}

void AppDocument::setListContours(std::string path, float pix, int bonusX, int bonusY)
{
	DcmFileFormat fileformat;
	OFCondition status = fileformat.loadFile(path.c_str());
	DcmItem *roiitem = NULL;
	DcmItem *contouritem = NULL;
	DcmItem *contourItemSequence = NULL;
	signed long ri = 0;
	Contours mContour;

	while (fileformat.getDataset()->findAndGetSequenceItem(DCM_ROIContourSequence, roiitem, ri++).good())
	{
		// roiitem now points to one item in the ROIContourSequence
		int idx = 0;
		signed long ci = 0;

		while (roiitem->findAndGetSequenceItem(DCM_ContourSequence, contouritem, ci++).good())
		{
			Volume vol;
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
						point.x = (int)((float)points[idx] + bonusX * pix ) / pix ;
						point.y = (int)((float)points[idx + 1] + bonusY * pix) / pix;
						mContour.addPoint(point);
					}
					AppDocument::listContour.push_back(mContour);
					mContour.clearContour();
				}
			}
		}
	}
}

std::vector<Contours> &AppDocument::getListContour()
{
	return listContour;
}

void AppDocument::setListVolume(std::string path, float pix, int bonusX, int bonusY)
{
	DcmFileFormat fileformat;
	OFCondition status = fileformat.loadFile(path.c_str());
	DcmItem *roiitem = NULL;
	DcmItem *contouritem = NULL;
	DcmItem *contourItemSequence = NULL;
	signed long ri = 0;
	Contours mContour;
	std::vector<std::vector<Contours>> listContourVol;
	std::vector< cv::Scalar> listColor;
	while (fileformat.getDataset()->findAndGetSequenceItem(DCM_ROIContourSequence, roiitem, ri++).good())
	{
		std::vector<Contours> listConImage;
		// roiitem now points to one item in the ROIContourSequence
		int idx = 0;
		signed long ci = 0;
		listConImage.clear();
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
					for (int idx = 0; idx < points.size(); idx += 3)
					{
						cv::Point point;
						point.x = (int)((float)points[idx] + bonusX * pix) / pix;
						point.y = (int)((float)points[idx + 1] + bonusY * pix) / pix;
						mContour.addPoint(point);
					}
					listConImage.push_back(mContour);
					mContour.clearContour();
				}
			}
		}
		listContourVol.push_back(listConImage);
		OFString color;
		int colors[3];
		signed long cl = 0;
		int k = 0;
		while (roiitem->findAndGetOFString(DCM_ROIDisplayColor, color, cl++).good())
		{
			colors[k] = std::stoi(color.c_str());
			k++;
		}
		listColor.push_back(cv::Scalar(colors[0], colors[1], colors[2]));
	}

	signed long i = 0;
	while (fileformat.getDataset()->findAndGetSequenceItem(DCM_RTROIObservationsSequence, roiitem, i++).good())
	{
		OFString name;
		OFString des;
		Sint32 reNumber;
		unsigned long j = 0;
		unsigned long k = 0;
		Volume vol;

		while (roiitem->findAndGetOFString(DCM_ROIObservationLabel, name, k++).good())
		{
			roiitem->findAndGetSint32(DCM_ReferencedROINumber, reNumber);
			vol.setRoiName(name.c_str());
			vol.setRoiNumber(reNumber);
		}
		while (roiitem->findAndGetOFString(DCM_RTROIInterpretedType, name, j++).good())
		{
			if (std::string(name.c_str()) == "MARKER")
			{
				roiitem->findAndGetOFString(DCM_ROIObservationDescription, des);
				roiitem->findAndGetSint32(DCM_ReferencedROINumber, reNumber);
				vol.setRoiName(name.c_str());
				vol.setRoiNumber(reNumber);
			}
		}
		vol.setListContour(listContourVol[i - 1]);
		vol.setRoiColor(listColor[i - 1]);
		listVolume.push_back(vol);
	}
}

std::vector<Volume> &AppDocument::getListVolume()
{
	return listVolume;
}


void AppDocument::setList(std::vector<Image> &listImg)
{
	for (int idxImg = 0; idxImg < listImg.size(); idxImg++)
	{
		Slice *oneSlice = new Slice();
		std::vector<Contours*> oneCon;
		oneSlice->setImage(&listImg[idxImg]);
		for (int idxListVolume = 0; idxListVolume < listVolume.size(); idxListVolume++)
		{
			for (int idxContour = 0; idxContour < listVolume[idxListVolume].getListContours().size(); idxContour++)
			{
				if (listImg[idxImg].getSOPInstanceUID() == listVolume[idxListVolume].getListContours()[idxContour].getReferencedUID())
				{
					oneCon.push_back(&listVolume[idxListVolume].getListContours()[idxContour]);
				}
			}
		}
		oneSlice->setListContour(oneCon);
		listMap.push_back(*oneSlice);
		oneCon.erase(oneCon.begin(), oneCon.end());
	}
}

std::vector<Slice> &AppDocument::getListMap()
{
	return listMap;
}

std::vector<Contours> AppDocument::getContourSlice(Slice &slice, Volume &volume)
{
	std::vector<Contours> contourVol;
	for (int idxSlice = 0; idxSlice < slice.getListContour().size(); idxSlice++)
	{
		for (int idxVolume = 0; idxVolume < volume.getListContours().size(); idxVolume++)
		{
			if (slice.getListContour()[idxSlice]->getReferencedUID() == volume.getListContours()[idxVolume].getReferencedUID())
			{
				contourVol.push_back(*slice.getListContour()[idxSlice]);
			}
		}
	}
	return contourVol;
}

void AppDocument::setListExternal(std::vector<Image> &listImg)
{
	std::vector<Contours> listContour;
	for (int index = 0; index < listImg.size(); index++)
	{
		Slice *oneSlice = new Slice();
		std::vector<Contours*> oneCon;
		oneSlice->setImage(&listImg[index]);
		cv::Mat thresholdedImage;
		//Apply canny to the input image
		//Canny(image, thresholdedImage, 50, 150);
		cv::threshold(listImg[index].getMatImage(), thresholdedImage, 52, 255, 0);

		cv::Mat kernel_02 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5), cv::Point(3, 3));
		cv::morphologyEx(thresholdedImage, thresholdedImage, cv::MORPH_OPEN, kernel_02);
		//imshow("iii", thresholdedImage);
		std::vector<std::vector<cv::Point>> contours;

		//To store hierarchy(nestedness)
		std::vector<cv::Vec4i> hierarchy;

		//Find contours
		findContours(thresholdedImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
		
		listConExt.push_back(contours);

		std::vector<cv::Point>cont;
		for (int i = 0; i < contours.size(); i++)
		{
			for (int j = 0; j < contours[i].size(); j++)
			{
				cont.push_back(contours[i][j]);
			}
		}
		Contours add;
		add.setDataContour(cont);
		cont.clear();
		oneCon.push_back(&add);
		oneSlice->setListContour(oneCon);
		listExternal.push_back(*oneSlice);
		add.clearContour();
	}
}

std::vector<Slice> &AppDocument::getListExternal()
{
	return listExternal;
}

std::vector<std::vector<std::vector<cv::Point>>> &AppDocument::getlistConExt()
{
	return listConExt;
}