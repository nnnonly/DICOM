// CProgressDialog.cpp : implementation file
//

#include "pch.h"
#include "DICOM.h"
#include "AppDocument.h"
#include "CProgressDialog.h"
#include "afxdialogex.h"
#include "ImageSlices.h"


// CProgressDialog dialog

IMPLEMENT_DYNAMIC(CProgressDialog, CDialogEx)

CProgressDialog::CProgressDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(ID_PROGRESSDIALOG, pParent)
{

}

CProgressDialog::~CProgressDialog()
{
}

void CProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_PROGRESS, m_Progress);
}

BOOL CProgressDialog::OnInitDialog()
{
	HMENU hSysMenu = ::GetSystemMenu(m_hWnd, FALSE);
	if (hSysMenu)
		::EnableMenuItem(hSysMenu, SC_CLOSE, (MF_DISABLED | MF_GRAYED | MF_BYCOMMAND));
	AppDocument* current = AppDocument::getInstance();
	CDialogEx::OnInitDialog();
	m_Progress.SetRange(0, 100);
	//m_Progress.SetPos(80);
	/*for (float z = 1; z < current->getImgData()[0].cols; z += current->getImage().getResize())
	{
		m_Progress.SetPos((int)(100/512)*z);
	}*/
	//
	SetTimer(999, 1000, nullptr);

	return TRUE;
}

void CProgressDialog::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 999) {
		if (m_Progress.GetPos() >= 99)
		{
			KillTimer(999);
			EndDialog(0);
		}
	}
}

void CProgressDialog::runThread()
{
	AfxBeginThread(ImageSlices::sagittalImage, &m_Progress);
	AfxBeginThread(ImageSlices::coronalImage, &m_Progress);
}

BEGIN_MESSAGE_MAP(CProgressDialog, CDialogEx)
ON_WM_TIMER()
END_MESSAGE_MAP()


// CProgressDialog message handlers
	