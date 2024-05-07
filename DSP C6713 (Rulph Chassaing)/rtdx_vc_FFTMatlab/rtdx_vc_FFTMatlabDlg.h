// rtdx_vc_FFTMatlabDlg.h : header file
//

#if !defined(AFX_RTDX_VC_FFTMATLABDLG_H__DE60BDA6_7784_11D8_8735_000347A3D902__INCLUDED_)
#define AFX_RTDX_VC_FFTMATLABDLG_H__DE60BDA6_7784_11D8_8735_000347A3D902__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CRtdx_vc_FFTMatlabDlg dialog

#include "RTDXThread.h"
class CRtdx_vc_FFTMatlabDlg : public CDialog
{
// Construction
public:
	static UINT RTDXThreadFunction(LPVOID lpvoid);
	CRTDXThread* pRTDXThread;
	CRtdx_vc_FFTMatlabDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRtdx_vc_FFTMatlabDlg)
	enum { IDD = IDD_RTDX_VC_FFTMATLAB_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRtdx_vc_FFTMatlabDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRtdx_vc_FFTMatlabDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTDX_VC_FFTMATLABDLG_H__DE60BDA6_7784_11D8_8735_000347A3D902__INCLUDED_)
