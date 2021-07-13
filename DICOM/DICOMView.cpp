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

// DICOMView.cpp : implementation of the CDICOMView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.

#ifndef SHARED_HANDLERS
#include "DICOM.h"
#endif

#include "DICOMDoc.h"
#include "DICOMView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDICOMView

IMPLEMENT_DYNCREATE(CDICOMView, CView)

BEGIN_MESSAGE_MAP(CDICOMView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	//ON_COMMAND(ID_INFORMATION, &CDICOMView::OnCLickInformation)
	//ON_COMMAND(ID_NEWPATIENT, &CDICOMView::OnClickNewPatient)
	//ON_COMMAND(ID_OPENPATIENT, &CDICOMView::OnClickOpenPatient)
END_MESSAGE_MAP()

// CDICOMView construction/destruction

CDICOMView::CDICOMView() noexcept
{
	// TODO: add construction code here

}

CDICOMView::~CDICOMView()
{
}

BOOL CDICOMView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDICOMView drawing

void CDICOMView::OnDraw(CDC*pDC /*pDC*/)
{
	//CDICOMDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	//if (!pDoc)
	//	return;

	//if (pDoc->image.empty())
	//{
	//	return;
	//}

	//CRect rect;
	//GetClientRect(&rect);
	//// GetDlgItem(ID_PICTURE_CONTROL_1)->GetClientRect(&rect);

	//// CDC *pDC = GetDlgItem(ID_PICTURE_CONTROL_1)->GetDC();
	//if (pDC == nullptr)
	//{
	//	return;
	//}

	//switch (pDoc->image.channels())
	//{
	//case 1:
	//	cv::cvtColor(pDoc->image, pDoc->image, CV_GRAY2BGRA);
	//	break;
	//case 3:
	//	cv::cvtColor(pDoc->image, pDoc->image, CV_BGR2BGRA);
	//	break;
	//default:
	//	break;
	//}

	//int pixelBytes = pDoc->image.channels()*(pDoc->image.depth() + 1);
	//
	//BITMAPINFO bitInfo;
	//bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
	//bitInfo.bmiHeader.biWidth = pDoc->image.cols;
	//bitInfo.bmiHeader.biHeight = -pDoc->image.rows;
	//bitInfo.bmiHeader.biPlanes = 1;
	//bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	//bitInfo.bmiHeader.biCompression = BI_RGB;
	//bitInfo.bmiHeader.biClrImportant = 0;
	//bitInfo.bmiHeader.biClrUsed = 0;
	//bitInfo.bmiHeader.biSizeImage = 0;
	//bitInfo.bmiHeader.biXPelsPerMeter = 0;
	//bitInfo.bmiHeader.biYPelsPerMeter = 0;

	//::StretchDIBits(
	//	pDC->GetSafeHdc(),
	//	0, 0, rect.Width(), rect.Height(),
	//	0, 0, pDoc->image.cols, pDoc->image.rows,
	//	pDoc->image.data,
	//	&bitInfo,
	//	DIB_RGB_COLORS,
	//	SRCCOPY
	//);

	// ReleaseDC(pDC);
}

void CDICOMView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDICOMView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDICOMView diagnostics

#ifdef _DEBUG
void CDICOMView::AssertValid() const
{
	CView::AssertValid();
}

void CDICOMView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDICOMDoc* CDICOMView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDICOMDoc)));
	return (CDICOMDoc*)m_pDocument;
}
#endif //_DEBUG


// CDICOMView message handlers


void CDICOMView::OnCLickInformation()
{
	// TODO: Add your command handler code here
	MessageBox(_T("This is my button"));
	//CEdit* editbox = (CEdit*)GetDlgItem(ID_INFORMATION);
	CWnd *label_name = GetDlgItem(ID_PATIENTNAME);
	label_name->SetWindowText(_T("abc"));
	UpdateData(TRUE);
	//SetDlgItemText(ID_PATIENTNAME, _T("Desired Text String"));
}


void CDICOMView::OnClickNewPatient()
{
	// TODO: Add your command handler code here
}


void CDICOMView::OnClickOpenPatient()
{
	// TODO: Add your command handler code here
}
