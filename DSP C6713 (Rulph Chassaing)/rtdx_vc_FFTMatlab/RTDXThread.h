#if !defined(AFX_RTDXTHREAD_H__DE60BDAE_7784_11D8_8735_000347A3D902__INCLUDED_)
#define AFX_RTDXTHREAD_H__DE60BDAE_7784_11D8_8735_000347A3D902__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RTDXThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRTDXThread thread

class CRTDXThread : public CWinThread
{
	DECLARE_DYNCREATE(CRTDXThread)
protected:
	CRTDXThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRTDXThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CRTDXThread();

	// Generated message map functions
	//{{AFX_MSG(CRTDXThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTDXTHREAD_H__DE60BDAE_7784_11D8_8735_000347A3D902__INCLUDED_)
