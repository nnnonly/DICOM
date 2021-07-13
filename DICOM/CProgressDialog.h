#pragma once


// CProgressDialog dialog

class CProgressDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CProgressDialog)

public:
	CProgressDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CProgressDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = ID_PROGRESSDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_Progress;
	void runThread();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
