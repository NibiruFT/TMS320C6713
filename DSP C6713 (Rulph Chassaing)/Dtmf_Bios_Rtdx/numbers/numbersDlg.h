// numbersDlg.h : header file
//

#if !defined(AFX_NUMBERSDLG_H__F7929AF9_5415_4AFC_9019_59E19AAB9E6C__INCLUDED_)
#define AFX_NUMBERSDLG_H__F7929AF9_5415_4AFC_9019_59E19AAB9E6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RtdxThread.h"

/////////////////////////////////////////////////////////////////////////////
// CNumbersDlg dialog

class CNumbersDlg : public CDialog
{
// Construction
public:
	static UINT RtdxPolling(LPVOID lpvoid);
	CNumbersDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNumbersDlg)
	enum { IDD = IDD_NUMBERS_DIALOG };
	CString	m_value;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNumbersDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CRtdxThread* pRtdxThread;
	HICON m_hIcon;
	CString m_RTDX;

	afx_msg LRESULT ProcessRtdx(WPARAM wParam, LPARAM lParam);

	// Generated message map functions
	//{{AFX_MSG(CNumbersDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnClear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NUMBERSDLG_H__F7929AF9_5415_4AFC_9019_59E19AAB9E6C__INCLUDED_)
