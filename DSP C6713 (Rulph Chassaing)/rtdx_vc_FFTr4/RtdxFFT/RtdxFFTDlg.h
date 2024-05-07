// RtdxFFTDlg.h : header file
//

#if !defined(AFX_RTDXFFTDLG_H__7096ED6A_51A2_498D_9E3C_BC73D7931D65__INCLUDED_)
#define AFX_RTDXFFTDLG_H__7096ED6A_51A2_498D_9E3C_BC73D7931D65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RTDXThread.h"

/////////////////////////////////////////////////////////////////////////////
// CRtdxFFTDlg dialog

class CRtdxFFTDlg : public CDialog
{
// Construction
public:
	CRtdxFFTDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRtdxFFTDlg)
	enum { IDD = IDD_RTDXFFT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRtdxFFTDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	static UINT RTDXThreadFunction(LPVOID lpVoid);
protected:
	HICON m_hIcon;
	CRTDXThread* pRTDXThread;

	// Generated message map functions
	//{{AFX_MSG(CRtdxFFTDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTDXFFTDLG_H__7096ED6A_51A2_498D_9E3C_BC73D7931D65__INCLUDED_)
