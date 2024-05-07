// VC_FIR_RTDX.h : main header file for the VC_FIR_RTDX application
//

#if !defined(AFX_VC_FIR_RTDX_H__C37E8688_2B01_4623_9C0C_D0DA209FE7F7__INCLUDED_)
#define AFX_VC_FIR_RTDX_H__C37E8688_2B01_4623_9C0C_D0DA209FE7F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CVC_FIR_RTDXApp:
// See VC_FIR_RTDX.cpp for the implementation of this class
//

class CVC_FIR_RTDXApp : public CWinApp
{
public:
	CVC_FIR_RTDXApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVC_FIR_RTDXApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVC_FIR_RTDXApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VC_FIR_RTDX_H__C37E8688_2B01_4623_9C0C_D0DA209FE7F7__INCLUDED_)
