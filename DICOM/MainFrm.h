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

// MainFrm.h : interface of the CMainFrame class
//


#pragma once


class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	//virtual BOOL OnCreateClient(LPCREATESTRUCT  lpcs, CCreateContext*  pContext);
#endif

protected:  // control bar embedded members
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	CStatic m_pictureControl_1;
	CStatic m_pictureControl_2;
	CStatic m_pictureControl_3;
	CSplitterWnd m_mainSplitter;
	BOOL m_bInitSplitter;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
	

public:
	afx_msg void OnInformation();
	afx_msg void OnNewpatient();
	afx_msg void OnOpenpatient();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//afx_msg void CMainFrame::OnDetectMarker();
	cv::Mat fitImage( const cv::Mat &img,
					  int winWidth,
					  int winHeight );
	void loadImage(cv::Mat img, int ID);
	void updateView();
	int checkInside(CPoint point);
	void loadInformation();
	//void updateImg();

	afx_msg void OnCreateXquang();
	afx_msg void OnDetectMarker();
	//afx_msg void OnFindContours();
	afx_msg void OnClickExternal();
};