// rtdx_vc_sine.h : main header file for the RTDX_VC_SINE application
//

#if !defined(AFX_RTDX_VC_SINE_H__179B5106_6C83_11D8_8735_000347A3D902__INCLUDED_)
#define AFX_RTDX_VC_SINE_H__179B5106_6C83_11D8_8735_000347A3D902__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRtdx_vc_sineApp:
// See rtdx_vc_sine.cpp for the implementation of this class
//

class CRtdx_vc_sineApp : public CWinApp
{
public:
	CRtdx_vc_sineApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRtdx_vc_sineApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRtdx_vc_sineApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTDX_VC_SINE_H__179B5106_6C83_11D8_8735_000347A3D902__INCLUDED_)
