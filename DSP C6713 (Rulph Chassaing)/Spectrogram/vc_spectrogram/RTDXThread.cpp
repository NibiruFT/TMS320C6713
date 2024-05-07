// RTDXThread.cpp : implementation file
//

#include "stdafx.h"
#include "vc_spectrogram.h"
#include "RTDXThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRTDXThread

IMPLEMENT_DYNCREATE(CRTDXThread, CWinThread)

CRTDXThread::CRTDXThread()
{
}

CRTDXThread::~CRTDXThread()
{
}

BOOL CRTDXThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CRTDXThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CRTDXThread, CWinThread)
	//{{AFX_MSG_MAP(CRTDXThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRTDXThread message handlers
