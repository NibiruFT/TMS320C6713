// VC_FIR_RTDXDlg.h : header file
//

#if !defined(AFX_VC_FIR_RTDXDLG_H__0DDA8154_F74A_433E_A4B7_A4D310230D45__INCLUDED_)
#define AFX_VC_FIR_RTDXDLG_H__0DDA8154_F74A_433E_A4B7_A4D310230D45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rtdxint.h"

/////////////////////////////////////////////////////////////////////////////
// CVC_FIR_RTDXDlg dialog

class CVC_FIR_RTDXDlg : public CDialog
{
// Construction
public:
	CVC_FIR_RTDXDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CVC_FIR_RTDXDlg)
	enum { IDD = IDD_VC_FIR_RTDX_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVC_FIR_RTDXDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	IRtdxExp *pRTDX;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVC_FIR_RTDXDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLoadFile();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VC_FIR_RTDXDLG_H__0DDA8154_F74A_433E_A4B7_A4D310230D45__INCLUDED_)
