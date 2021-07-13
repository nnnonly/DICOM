// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MainFrm.cpp : implementation of the CMainFrame class
//
#include "pch.h"
#include "framework.h"
#include "DICOM.h"
#include "DICOMDoc.h"
#include "DICOMView.h"
#include "MainFrm.h"
#include "WindowController.h"
#include "ImageSlices.h"
#include "DetectMarker.h"
#include "CProgressDialog.h"
#include <thread>
#include <dirent.h>
#include <typeinfo>
#define RUNTIME_CLASS(class_name) (class_name::GetThisClass())

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define distance 148

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_INFORMATION, &CMainFrame::OnInformation)
	ON_COMMAND(ID_NEWPATIENT, &CMainFrame::OnNewpatient)
	ON_COMMAND(ID_OPENPATIENT, &CMainFrame::OnOpenpatient)
	ON_WM_MOUSEWHEEL(ID_PICTURE_CONTROL_1, &CMainFrame::OnMouseWheel)
	ON_COMMAND(ID_BUTTON2, &CMainFrame::OnCreateXquang)
	//ON_COMMAND(ID_BUTTON2, &CMainFrame::OnDetectMarker)
	//ON_COMMAND(ID_BUTTON3, &CMainFrame::OnDetectMarker)
	ON_COMMAND(EXTERNAL, &CMainFrame::OnClickExternal)
END_MESSAGE_MAP()

std::vector<cv::Mat> imageData;
std::vector<cv::Mat> testImg;
int upTrans = 0;
int upSal = 0;
int upCor = 0;
int upOpen = 0;
//std::vector<cv::Mat> folSagittal = ImageSlices::sagittalImage();
// CMainFrame construction/destruction

CMainFrame::CMainFrame() noexcept
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
}

CMainFrame::~CMainFrame()
{
	
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	// set the visual manager and style based on persisted value
	
	OnApplicationLook(theApp.m_nAppLook);

	 m_pictureControl_1.Create(NULL, WS_CHILD | WS_VISIBLE, CRect(3, 148, 700, 850), this, ID_PICTURE_CONTROL_1);
	 m_pictureControl_2.Create(NULL, WS_CHILD | WS_VISIBLE, CRect(3, 148, 700, 850), this, ID_PICTURE_CONTROL_2);
	 m_pictureControl_3.Create(NULL, WS_CHILD | WS_VISIBLE, CRect(3, 148, 700, 850), this, ID_PICTURE_CONTROL_3);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//picture control 1
	if (m_pictureControl_1.GetSafeHwnd() != NULL)
	{
		CRect r;
		GetWindowRect(&r);
		ScreenToClient(&r);
		m_pictureControl_1.SetWindowPos(NULL,
			0,
			distance,
			2*r.Width()/3, r.Height() - distance,
			SWP_NOZORDER);
	}

	//picture control 2
	if (m_pictureControl_2.GetSafeHwnd() != NULL)
	{ 
		CRect r;
		GetWindowRect(&r);
		ScreenToClient(&r);
		m_pictureControl_2.SetWindowPos(NULL,                      
			2 * r.Width() / 3 + 10,
			distance,                     
			1 * r.Width() / 3, (r.Height() - distance)/2 - 5,
			SWP_NOZORDER);
	}

	//picture control 3
	if (m_pictureControl_3.GetSafeHwnd() != NULL)
	{ 
		CRect r;
		GetWindowRect(&r);
		ScreenToClient(&r);
		m_pictureControl_3.SetWindowPos(NULL,
			2 * r.Width() / 3 + 10,
			distance + (r.Height() - distance) / 2,
			1 * r.Width() / 3, (r.Height() - distance) / 2 - 5,
			SWP_NOZORDER);
	}
}

void CMainFrame::OnInformation()
{
	// TODO: Add your command handler code here
	AppDocument *appDoc = AppDocument::getInstance();

	CMFCRibbonEdit* pEditPatientName = DYNAMIC_DOWNCAST(CMFCRibbonEdit,m_wndRibbonBar.FindByID(ID_PATIENTNAME_TXT));
	pEditPatientName->SetEditText(appDoc->getImage().getPatientName().c_str());

	CMFCRibbonEdit* pEditPatientID = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_PATIENTID_TXT));
	pEditPatientID->SetEditText(appDoc->getImage().getPatientID().c_str());

	CMFCRibbonEdit* pEditPatientBirthDate = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_BIRTHDATE_TXT));
	pEditPatientBirthDate->SetEditText(appDoc->getImage().getPatientBirthDate().c_str());

	CMFCRibbonEdit* pEditPatientSex = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_PATIENTSEX_TXT));
	pEditPatientSex->SetEditText(appDoc->getImage().getPatientSex().c_str());

	CMFCRibbonEdit* pEditPatientWeight = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_PATIENTWEIGHT_TXT));
	pEditPatientWeight->SetEditText(appDoc->getImage().getPatientWeight().c_str());

	CMFCRibbonEdit* pEditPatientAge = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_PATIENTAGE_TXT));
	pEditPatientAge->SetEditText(appDoc->getImage().getPatientAge().c_str());

}

void CMainFrame::OnNewpatient()
{
	// TODO: Add your command handler code here
}

void CMainFrame::loadImage(cv::Mat img, int ID)
{

	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);

	cv::resize(img, img, cv::Size(rect.Width(), rect.Height()));

	switch (img.channels())
	{
	case 1:
		cv::cvtColor(img, img, CV_GRAY2BGRA); //gray
		break;

	case 3:
		cv::cvtColor(img, img, CV_BGR2BGRA);
		break;

	default:
		break;
	}

	//tinh xem co bao nhieu byte trong 1 pixel
	int pixelBytes = img.channels() * (img.depth() + 1);
	BITMAPINFO bitInfor;

	bitInfor.bmiHeader.biBitCount = 8 * pixelBytes;
	bitInfor.bmiHeader.biWidth = img.cols;
	bitInfor.bmiHeader.biHeight = -img.rows;
	bitInfor.bmiHeader.biPlanes = 1;
	bitInfor.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfor.bmiHeader.biCompression = BI_RGB;
	bitInfor.bmiHeader.biClrImportant = 0;
	bitInfor.bmiHeader.biClrUsed = 0;
	bitInfor.bmiHeader.biSizeImage = 0;
	bitInfor.bmiHeader.biXPelsPerMeter = 0;
	bitInfor.bmiHeader.biYPelsPerMeter = 0;

	CDC *pDC = GetDlgItem(ID)->GetDC();

	::StretchDIBits(
		pDC->GetSafeHdc(),
		0, 0, rect.Width(), rect.Height(),
		0, 0, img.cols, img.rows,
		img.data,
		&bitInfor,
		DIB_RGB_COLORS,
		SRCCOPY
	);
	ReleaseDC(pDC);
}

void CMainFrame::updateView()
{
	CRect r;
	GetWindowRect(&r);
	ScreenToClient(&r);

	AppDocument* updateApp = AppDocument::getInstance();
	
	//if (updateApp->getImage().getMatImage().empty()) {
	//	return;
	//}

	//WindowController::loadPixel(std::atoi(updateApp->getImage().getWidth().c_str()), std::atoi(updateApp->getImage().getCenter().c_str()), updateApp->getImage().getMatImage(), out);
	//if (updateApp->getPointPlus().size() != 0 && updateApp->getPointMulti().size() != 0)
	//{
	//	DetectMarker::drawMarkerPlus(out, updateApp->getPointPlus());
	//	DetectMarker::drawMarkerMulti(out, updateApp->getPointMulti());
	//}
	cv::Mat out;
	std::vector<Volume> listVolume = updateApp->getListVolume();
	WindowController::loadPixel(std::atoi(updateApp->getListMap()[upTrans].getImage()->getWidth().c_str()), std::atoi(updateApp->getListMap()[upTrans].getImage()->getCenter().c_str()), updateApp->getListMap()[upTrans].getImage()->getMatImage(), out);
	cv::cvtColor(out, out, cv::COLOR_GRAY2BGR);
	for (Volume vol : updateApp->getListVolume())
	{
		std::vector<std::vector<cv::Point>> boxContours;
		//Volume voll = updateApp->getListVolume()[2];
		std::vector<Contours> conSlice = vol.getContourSlice(updateApp->getListMap()[upTrans]);
		
		for (int i = 0; i < conSlice.size(); i++)
		{
			boxContours.clear();
			boxContours.push_back(conSlice[i].getDataContour());
			cv::drawContours(out, boxContours, -1, vol.getRoiColor(), 1.5);
		}
		if (updateApp->getlistConExt().size() > 0)
		{
			cv::drawContours(out, updateApp->getlistConExt()[upTrans], -1, cv::Scalar(0,0,255), 1.5);
		}
		conSlice.clear();
	}

	cv::Mat picControl1 = fitImage(out, r.Height() - distance, 2 * r.Width() / 3);
	loadImage(picControl1, ID_PICTURE_CONTROL_1);
	cv::Mat picControl2 = fitImage(updateApp->getCoronalImage(), (r.Height() - distance) / 2 - 5, r.Width() / 3);
	loadImage(picControl2, ID_PICTURE_CONTROL_2);
	cv::Mat picControl3 = fitImage(updateApp->getSagittalImage(), (r.Height() - distance) / 2 - 5, r.Width() / 3);
	loadImage(picControl3, ID_PICTURE_CONTROL_3);
	//for (auto& v : boxContours) {
	//	v.clear();
	//}
	//delete boxContours;
}

void CMainFrame::OnOpenpatient()
{
	// TODO: Add your command handler code here
	CFolderPickerDialog dlgFolder;
	std::vector<std::string> folder;

	if (dlgFolder.DoModal() != IDOK)
	{
		return;
	}
	AppDocument* appDoc = AppDocument::getInstance();
	//appDoc->clearSagittal();
	testImg.clear();
	imageData.clear();
	CString filepath = dlgFolder.GetPathName();
	std::string sPath = filepath.GetString();
	DIR *dr;
	struct dirent *en;
	dr = opendir(sPath.c_str());
	if (dr) {
		while ((en = readdir(dr)) != NULL) {
			if (std::string(en->d_name).substr(std::string(en->d_name).find_last_of(".") + 1) == "dcm" || std::string(en->d_name).substr(std::string(en->d_name).find_last_of(".") + 1) == "DCM")
			{
				folder.push_back(std::string(en->d_name));
			}
		}
		closedir(dr); //close all directory
	}
	std::vector<float> sliceLocation;
	for (int i = 0; i < folder.size(); i++)
	{
		std::string path = sPath + "\\" + folder[i];
		cv::Mat img;
		Image transImage;
		WindowController::loadInforImage(path, img, transImage);
		imageData.push_back(img);
		appDoc->addTransImage(transImage);
		cv::Mat out;
		WindowController::loadPixel(std::atoi(appDoc->getImage().getWidth().c_str()), std::atoi(appDoc->getImage().getCenter().c_str()), appDoc->getImage().getMatImage(), out);
		testImg.push_back(out);
		//sliceLocation.push_back(appDoc->getSliceLocation());
	}
	//for (int i = 0; i < sliceLocation.size() - 1; i++)
	//{
	//	for (int j = i + 1; j < sliceLocation.size(); j++)
	//	{
	//		float x = sliceLocation[i];
	//		float y = sliceLocation[j];
	//		if (sliceLocation[i] < sliceLocation[j])
	//		{
	//			std::swap(sliceLocation[i], sliceLocation[j]);
	//			std::swap(imageData[i], imageData[j]);
	//			std::swap(testImg[i], testImg[j]);
	//		}
	//	}
	//}
	//AppDocument* appDoc = AppDocument::getInstance();
	appDoc->setImgData(testImg);
	appDoc->getImage().setMatImage(imageData[0]);

	//ImageSlices::run();
	/*ImageSlices::sagittalImage();
	ImageSlices::coronalImage();*/
	upSal = appDoc->getImage().getLocation();
	upCor = appDoc->getImage().getLocation();
	//appDoc->setListContours("D:\\Folder Dicom Image\\DICOM\\RT_Structure_Set_Storage-Abdominal_3001__-Patient_Model_1--", appDoc->getImage().getColumnPixelSpacing(), 512 / 2, 512 / 2);
	appDoc->setListVolume("D:\\Folder Dicom Image\\\Head_0064\\2_23_2021_12_00_00_AM\\RT_Structure_Set_StorageHead_0064__Patient_Model-", appDoc->getImage().getColumnPixelSpacing(), 200, 200);

	appDoc->setList(appDoc->getTransverseImage());
	CProgressDialog dlg;
	dlg.runThread();
	dlg.DoModal();
	//Sleep(2000);
	//appDoc->setListMap(appDoc->getTransverseImage(), appDoc->getListContour());
	updateView();
}

cv::Mat CMainFrame::fitImage(const cv::Mat &img, int winWidth, int winHeight)
{
	int width = img.cols,
		height = img.rows;

	cv::Mat imageFrame = cv::Mat::zeros(winWidth, winHeight, img.type());
	cv::Mat imgResize;
	if (winWidth > winHeight)
	{
		cv::resize(img, imgResize, cv::Size(winHeight, winHeight));
	}
	else
	{
		cv::resize(img, imgResize, cv::Size(winWidth, winWidth));
	}

	cv::Mat destRoi;
	int a = winHeight - imgResize.cols;
	destRoi = imageFrame(cv::Rect((winHeight - imgResize.cols)/2, 0, imgResize.cols, imgResize.rows));
	imgResize.copyTo(destRoi);
	return imageFrame;
}

BOOL CMainFrame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int inside = checkInside(pt);
	AppDocument* appDoc = AppDocument::getInstance();
	if (inside == 1)
	{
		if (zDelta > 0)
		{
			if (upTrans + 1 < imageData.size())
			{
				upTrans += 1;
				appDoc->getImage().setMatImage(imageData[upTrans]);
				updateView();
			}
		}
		else
		{
			if (upTrans - 1 >= 0)
			{
				upTrans -= 1;
				appDoc->getImage().setMatImage(imageData[upTrans]);
				updateView();
			}
		}
	}
	else if (inside == 2)
	{
		std::vector<cv::Mat> folCoronalImg = appDoc->getFolCoronalImage();
		if (zDelta > 0)
		{
			if (upCor < folCoronalImg.size() - 1)
			{
				appDoc->setCoronalImage(folCoronalImg[upCor]);
				updateView();
				upCor += 1;
			}
		}
		else
		{
			if (upCor - 1 >= 0)
			{
				appDoc->setCoronalImage(folCoronalImg[upCor]);
				updateView();
				upCor -= 1;
			}
		}
	}
	else if (inside == 3)
	{
		std::vector<cv::Mat> folSagittalImg = appDoc->getFolSagittalImage();
		cv::Mat abc = appDoc->getFolSagittalImage().at(0);
		if (zDelta > 0)
		{
			if (upSal < folSagittalImg.size() - 1)
			{
				appDoc->setSagittalImage(folSagittalImg[upSal]);
				updateView();
				upSal += 1;
			}
		}
		else
		{
			if (upSal - 1 >= 0)
			{
				appDoc->setSagittalImage(folSagittalImg[upSal]);
				updateView();
				upSal -= 1;
			}
		}
	}
	return CFrameWndEx::OnMouseWheel(nFlags, zDelta, pt);
}

int CMainFrame::checkInside(CPoint point)
{
	LPPOINT pt1 = new POINT();
	pt1->x = point.x;
	pt1->y = point.y;

	CRect windowRectPC1;
	CRect windowRectPC2;
	CRect windowRectPC3;
	m_pictureControl_1.GetWindowRect(windowRectPC1);
	m_pictureControl_2.GetWindowRect(windowRectPC2);
	m_pictureControl_3.GetWindowRect(windowRectPC3);

	if (windowRectPC1.PtInRect(*pt1))
	{
		return 1;
	}
	else if (windowRectPC2.PtInRect(*pt1))
	{
		return 2;
	}
	else if (windowRectPC3.PtInRect(*pt1))
	{
		return 3;
	}
	else
	{
		return 0;
	}
}

//BOOL  CMainFrame::OnCreateClient(LPCREATESTRUCT  lpcs, CCreateContext*  pContext)
//{
//	//calculate client size
//	CRect cr;
//	GetWindowRect(&cr);
//
//	// Create the main splitter with 1 row and 2 columns
//	if (!m_mainSplitter.CreateStatic(this, 1, 2))
//	{
//		MessageBox("Error setting up m_mainSplitter", "ERROR", MB_OK | MB_ICONERROR);
//		return FALSE;
//	}
//
//	// The views for each pane must be created 
//	if (!m_mainSplitter.CreateView(0, 0, RUNTIME_CLASS(CSplitterWindowTutorialView),
//		CSize(cr.Width() / 2, cr.Height()), pContext))
//	{
//		MessageBox("Error setting up splitter view", "ERROR", MB_OK | MB_ICONERROR);
//		return FALSE;
//	}
//
//	if (!m_mainSplitter.CreateView(0, 1, RUNTIME_CLASS(CSplitterWindowTutorialView),
//		CSize(cr.Width() / 2, cr.Height()), pContext))
//	{
//		MessageBox("Error setting up splitter view", "ERROR", MB_OK | MB_ICONERROR);
//		return FALSE;
//	}
//
//	//change flag to show splitter created
//	m_bInitSplitter = true;
//
//	//return TRUE instead of the parent method since that would
//	//not show our window
//	return TRUE;
//}


void CMainFrame::OnCreateXquang()
{
	// TODO: Add your command handler code here
	AppDocument* current = AppDocument::getInstance();
	auto& imageSagittal = current->getFolSagittalImage();
	int index = 0;
	float sum = 0;
	float denom = imageSagittal.size();
	std::vector<float> checkData;

	cv::Mat xImage = cv::Mat::zeros(imageSagittal[0].rows, imageSagittal[0].cols, imageSagittal[0].type());

	for (int row = 0; row < imageSagittal[index].rows; row++)
	{
		for (int col = 0; col < imageSagittal[index].cols; col++)
		{
			sum = 0;
			float check = imageSagittal[1].data[255 * imageSagittal[1].cols + col];
			for (int idx = 0; idx < imageSagittal.size(); idx++)
			{
				sum += imageSagittal[idx].data[row*imageSagittal[idx].cols + col];
			}
			xImage.data[row*imageSagittal[index].cols + col] = sum / denom;
		}
	}
	cv::Mat out;
	xImage.convertTo(out, -1, 5, 0);
	cv::imshow("xquang", out);
}

void CMainFrame::OnDetectMarker()
{
	// TODO: Add your command handler code here
	AppDocument* current = AppDocument::getInstance();
	cv::Mat cloneImg = current->getImage().getMatImage();
	cv::Mat detectImg;
	
	WindowController::loadPixel(std::atoi(current->getImage().getWidth().c_str()), std::atoi(current->getImage().getCenter().c_str()), current->getImage().getMatImage(), detectImg);
	cv::Mat outDetectImg = detectImg.clone();
	
	std::vector<cv::Point2f> centerPointPlus;
	std::vector<cv::Point2f> centerPointMulti;
	
	DetectMarker::preProcessing(detectImg);
	cv::Mat maskPlus;
	DetectMarker::GeneratePlusMask(maskPlus, current->getImage().getRowPixelSpacing(), current->getImage().getColumnPixelSpacing(), 9, 9);
	
	cv::Mat maskMulti;
	DetectMarker::GenerateMultiMask(maskMulti, current->getImage().getRowPixelSpacing(), current->getImage().getColumnPixelSpacing(), 9, 9);
	
	DetectMarker::featureExtraction(detectImg, maskPlus, maskMulti, centerPointPlus, centerPointMulti);
	
	std::vector<cv::Point2f> drawPlus = DetectMarker::postProcessingPlus(detectImg, centerPointPlus);
	current->setPointPlus(drawPlus);
	
	std::vector<cv::Point2f> drawMulti = DetectMarker::postProcessingMulti(detectImg, centerPointMulti);
	current->setPointMulti(drawMulti);
}


void CMainFrame::OnClickExternal()
{
	// TODO: Add your command handler code here
	AppDocument* current = AppDocument::getInstance();
	current->setListExternal(current->getTransverseImage());
	updateView();
}
