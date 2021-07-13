#include "pch.h"
#include "Slice.h"

void Slice::setImage(Image *image)
{
	this->image = image;
}

Image *Slice::getImage()
{
	return image;
}

void Slice::setListContour(std::vector<Contours*> listContour)
{
	this->listContour = listContour;
}

std::vector<Contours*> Slice::getListContour()
{
	return listContour;
}
