// rtdx_vc_sineDlg.h : header file
//

#if !defined(AFX_RTDX_VC_SINEDLG_H__179B5108_6C83_11D8_8735_000347A3D902__INCLUDED_)
#define AFX_RTDX_VC_SINEDLG_H__179B5108_6C83_11D8_8735_000347A3D902__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rtdxint.h"

/////////////////////////////////////////////////////////////////////////////
// CRtdx_vc_sineDlg dialog

class CRtdx_vc_sineDlg : public CDialog
{
// Construction
public:
	IRtdxExp* pRTDX;
	CRtdx_vc_sineDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRtdx_vc_sineDlg)
	enum { IDD = IDD_RTDX_VC_SINE_DIALOG };
	int		m_slider;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRtdx_vc_sineDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRtdx_vc_sineDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTDX_VC_SINEDLG_H__179B5108_6C83_11D8_8735_000347A3D902__INCLUDED_)
