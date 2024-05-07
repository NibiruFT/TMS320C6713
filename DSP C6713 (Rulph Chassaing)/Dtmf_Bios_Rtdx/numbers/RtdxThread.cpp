// RtdxThread.cpp : implementation file
//

#include "stdafx.h"
#include "numbers.h"
#include "RtdxThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRtdxThread

IMPLEMENT_DYNCREATE(CRtdxThread, CWinThread)

CRtdxThread::CRtdxThread()
{
}

CRtdxThread::~CRtdxThread()
{
}

BOOL CRtdxThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CRtdxThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CRtdxThread, CWinThread)
	//{{AFX_MSG_MAP(CRtdxThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRtdxThread message handlers
