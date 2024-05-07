// vc_spectrogramDlg.h : header file
//

#if !defined(AFX_VC_SPECTROGRAMDLG_H__0845237D_6BFE_4298_A51A_D37F8A3F4B8E__INCLUDED_)
#define AFX_VC_SPECTROGRAMDLG_H__0845237D_6BFE_4298_A51A_D37F8A3F4B8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RTDXThread.h"

/////////////////////////////////////////////////////////////////////////////
// CVc_spectrogramDlg dialog

class CVc_spectrogramDlg : public CDialog
{
// Construction
public:
	CVc_spectrogramDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CVc_spectrogramDlg)
	enum { IDD = IDD_VC_SPECTROGRAM_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVc_spectrogramDlg)
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
	//{{AFX_MSG(CVc_spectrogramDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VC_SPECTROGRAMDLG_H__0845237D_6BFE_4298_A51A_D37F8A3F4B8E__INCLUDED_)
