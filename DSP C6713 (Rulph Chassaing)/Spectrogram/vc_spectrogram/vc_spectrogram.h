// vc_spectrogram.h : main header file for the VC_SPECTROGRAM application
//

#if !defined(AFX_VC_SPECTROGRAM_H__9178E071_5243_4ED8_90C4_781E592E9F1E__INCLUDED_)
#define AFX_VC_SPECTROGRAM_H__9178E071_5243_4ED8_90C4_781E592E9F1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CVc_spectrogramApp:
// See vc_spectrogram.cpp for the implementation of this class
//

class CVc_spectrogramApp : public CWinApp
{
public:
	CVc_spectrogramApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVc_spectrogramApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVc_spectrogramApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VC_SPECTROGRAM_H__9178E071_5243_4ED8_90C4_781E592E9F1E__INCLUDED_)
