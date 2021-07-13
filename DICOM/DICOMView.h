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

// DICOMView.h : interface of the CDICOMView class
//



#pragma once

class CDICOMView : public CView
{
protected: // create from serialization only
	CDICOMView() noexcept;
	DECLARE_DYNCREATE(CDICOMView)

// Attributes
public:
	CDICOMDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CDICOMView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCLickInformation();

private:
	CButton m_information;
	CImage m_image;
	CStatic m_pictureControl;
public:
	afx_msg void OnClickNewPatient();
	afx_msg void OnClickOpenPatient();
};

#ifndef _DEBUG  // debug version in DICOMView.cpp
inline CDICOMDoc* CDICOMView::GetDocument() const
   { return reinterpret_cast<CDICOMDoc*>(m_pDocument); }
#endif

