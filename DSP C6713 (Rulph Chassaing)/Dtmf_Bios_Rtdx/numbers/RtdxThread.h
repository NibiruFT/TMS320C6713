#if !defined(AFX_RTDXTHREAD_H__247E66A1_BAFE_41F0_98D4_239873EFA70B__INCLUDED_)
#define AFX_RTDXTHREAD_H__247E66A1_BAFE_41F0_98D4_239873EFA70B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RtdxThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRtdxThread thread

class CRtdxThread : public CWinThread
{
	DECLARE_DYNCREATE(CRtdxThread)
protected:
	CRtdxThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRtdxThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CRtdxThread();

	// Generated message map functions
	//{{AFX_MSG(CRtdxThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTDXTHREAD_H__247E66A1_BAFE_41F0_98D4_239873EFA70B__INCLUDED_)
