// RtdxFFT.h : main header file for the RTDXFFT application
//

#if !defined(AFX_RTDXFFT_H__894561FB_6C94_4904_8FF4_EEBB9DF576E2__INCLUDED_)
#define AFX_RTDXFFT_H__894561FB_6C94_4904_8FF4_EEBB9DF576E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRtdxFFTApp:
// See RtdxFFT.cpp for the implementation of this class
//

class CRtdxFFTApp : public CWinApp
{
public:
	CRtdxFFTApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRtdxFFTApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRtdxFFTApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTDXFFT_H__894561FB_6C94_4904_8FF4_EEBB9DF576E2__INCLUDED_)
