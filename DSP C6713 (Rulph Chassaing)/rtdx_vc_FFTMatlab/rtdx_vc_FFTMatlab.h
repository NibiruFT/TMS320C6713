// rtdx_vc_FFTMatlab.h : main header file for the RTDX_VC_FFTMATLAB application
//

#if !defined(AFX_RTDX_VC_FFTMATLAB_H__DE60BDA4_7784_11D8_8735_000347A3D902__INCLUDED_)
#define AFX_RTDX_VC_FFTMATLAB_H__DE60BDA4_7784_11D8_8735_000347A3D902__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRtdx_vc_FFTMatlabApp:
// See rtdx_vc_FFTMatlab.cpp for the implementation of this class
//

class CRtdx_vc_FFTMatlabApp : public CWinApp
{
public:
	CRtdx_vc_FFTMatlabApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRtdx_vc_FFTMatlabApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRtdx_vc_FFTMatlabApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTDX_VC_FFTMATLAB_H__DE60BDA4_7784_11D8_8735_000347A3D902__INCLUDED_)
