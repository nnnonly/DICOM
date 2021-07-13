#include "pch.h"
#include "ImageSlices.h"
#include "AppDocument.h"

float pos = 0;

void rotate(const cv::Mat& src, float angle)
{
	cv::Mat dst;
	cv::Point2f pt(src.cols / 2., src.rows / 2.);
	cv::Mat r = getRotationMatrix2D(pt, angle, 1.0);
	warpAffine(src, src, r, cv::Size(src.cols, src.rows));
	//return dst;
}

void accumulator(unsigned int beginIdex, unsigned int endIndex)
{

}

//void ImageSlices::run()
//{
//	AfxBeginThread(sagittalImage, 0);
//	AfxBeginThread(coronalImage, 0);
//}

UINT ImageSlices::sagittalImage(LPVOID pParam)
{
	CProgressCtrl* progress = (CProgressCtrl*)pParam;
	AppDocument* current = AppDocument::getInstance();
	auto& imageData = current->getImgData();
	cv::Mat sagittalImg = cv::Mat::zeros(imageData.size(), imageData[0].cols, imageData[0].type());

	std::vector<cv::Mat> folTest;
	std::vector<uchar*> data;
	//static int index = 10;
	//index++;
	for (int size = 0; size < imageData.size(); size++)
	{
		data.push_back(imageData[size].data);
	}
	int cols = imageData[0].cols;
	int rows = imageData[0].rows;
	float abc = cols / (float)(current->getImage().getResize());
	for (float z = 1; z < cols; z += current->getImage().getResize())
	{
		for (int i = 0; i < imageData.size(); i++)
		{
			for (int j = 0; j < rows; j++)
			{
				//lam tron xuong
				int x1 = (int)floor(z);
				//lam tron len
				int x2 = (int)ceil(z);
				//y = y1 + (x-x1)((y2-y1)/(x2-x1))
				float pixelValue = 0;
				if (x1 == x2)
				{
					pixelValue = data[i][x1 * cols + j];
				}
				else
				{
					pixelValue = data[i][x1 * cols + j] + (z - x1) * ((data[i][x2 * cols + j] - data[i][x1 * cols + j]) / (x2 - x1));
				}
				sagittalImg.data[i*sagittalImg.cols + j] = pixelValue;
			}
		}
		cv::Mat imageSagittalResize;
		if (imageData.size() * current->getImage().getResize() < 512)
		{
			cv::resize(sagittalImg, imageSagittalResize, cv::Size(imageData[0].cols, imageData.size() * current->getImage().getResize()));
		}
		else
		{
			cv::resize(sagittalImg, imageSagittalResize, cv::Size(imageData[0].cols, imageData[0].rows));
		}
		

		cv::Mat imageFrame = cv::Mat::zeros(imageData[0].cols, imageData[0].rows, imageData[0].type());
		cv::Mat destRoi = imageFrame(cv::Rect((imageData[0].cols - imageSagittalResize.cols) / 2, (imageData[0].rows - imageSagittalResize.rows) / 2, imageSagittalResize.cols, imageSagittalResize.rows));
		imageSagittalResize.copyTo(destRoi);
		folTest.push_back(imageFrame);
		pos += 49 / abc;
		progress->SetPos(std::min((int)floor(pos), 98));
	}
	current->setFolSagittalImage(folTest);
	current->setSagittalImage(folTest[folTest.size() / 2 - 1]);
	current->getImage().setLocation(folTest.size() / 2 - 1);
	progress->SetPos(progress->GetPos() + 1);
	//Sleep(2000);
	return TRUE;
}

UINT ImageSlices::coronalImage(LPVOID pParam)
{
	CProgressCtrl* progress = (CProgressCtrl*)pParam;
	AppDocument* current = AppDocument::getInstance();
	auto& imageData = current->getImgData();
	cv::Mat coronalImage = cv::Mat::zeros(imageData[0].rows, imageData.size(), imageData[0].type());

	std::vector<cv::Mat> folCoronal;
	int rows = imageData[0].rows;
	float abc = rows / (float)(current->getImage().getResize());
	for (int z = 0; z < rows; z+= current->getImage().getResize())
	{
		for (int col = 0; col < imageData.size(); col++)
		{
			for (int row = 0; row < imageData[col].rows; row++)
			{
				int x1 = (int)floor(z);
				int x2 = (int)ceil(z);
				float pixelValue;
				if (x1 == x2)
				{
					pixelValue = imageData[col].data[row * imageData[col].cols + x1];
				}
				else
				{
					pixelValue = imageData[col].data[row * imageData[col].cols + x1] + (z - x1) * ((imageData[col].data[row * imageData[col].cols + x2] - imageData[col].data[row * imageData[col].cols + x1]) / (x2 - x1));
				}
				coronalImage.data[row*coronalImage.cols + col] = pixelValue;
			}
		}
		cv::Mat coronalImageResize;
		float bonus = (imageData.size() * current->getImage().getResize()) / imageData[0].cols;
		if (imageData[0].cols*bonus < imageData[0].cols)
		{
			cv::resize(coronalImage, coronalImageResize, cv::Size(imageData[0].cols*bonus, imageData[0].rows));
		}
		else
		{
			cv::resize(coronalImage, coronalImageResize, cv::Size(imageData[0].cols, imageData[0].rows));
		}
		cv::Mat imageFrame = cv::Mat::zeros(imageData[0].cols, imageData[0].rows, imageData[0].type());
		cv::Mat destRoi = imageFrame(cv::Rect((imageData[0].cols - coronalImageResize.cols) / 2, (imageData[0].rows - coronalImageResize.rows) / 2, coronalImageResize.cols, coronalImageResize.rows));
		coronalImageResize.copyTo(destRoi);
		rotate(imageFrame, -90.0);
		folCoronal.push_back(imageFrame);
		pos += 49/abc;
		progress->SetPos(std::min((int)floor(pos), 98));
	}
	current->setFolCoronalImage(folCoronal);
	current->setCoronalImage(folCoronal[folCoronal.size()/2 - 1]);
	
	progress->SetPos(progress->GetPos() + 1);
	//Sleep(2000);
	return TRUE;
}