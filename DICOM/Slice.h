#pragma once
#include "pch.h"
#include "Contours.h"
#include "Image.h"

class Slice
{
private:
	Image* image;
	std::vector<Contours*> listContour;

public:
	void setImage(Image *image);

	Image *getImage();

	void setListContour(std::vector<Contours*> listContour);

	std::vector<Contours*> getListContour();

};

