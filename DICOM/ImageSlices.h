#pragma once

#include "pch.h"
#include <thread>
namespace ImageSlices
{
	void run();
	UINT sagittalImage(LPVOID pParam);
	UINT coronalImage(LPVOID pParam);
};

