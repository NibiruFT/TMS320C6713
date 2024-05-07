// RtdxFFTDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RtdxFFT.h"
#include "RtdxFFTDlg.h"
#include "MatlabClass.h"
#include "Rtdxint.h"
#include <cmath>

#define RTDX_BUFFER_SIZE 256

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int nFlag = 1;

/////////////////////////////////////////////////////////////////////////////
// CRtdxFFTDlg dialog

CRtdxFFTDlg::CRtdxFFTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRtdxFFTDlg::IDD, pParent)
	, pRTDXThread(NULL)
{
	//{{AFX_DATA_INIT(CRtdxFFTDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRtdxFFTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRtdxFFTDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRtdxFFTDlg, CDialog)
	//{{AFX_MSG_MAP(CRtdxFFTDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRtdxFFTDlg message handlers

BOOL CRtdxFFTDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	pRTDXThread = (CRTDXThread*)AfxBeginThread(RTDXThreadFunction, m_hWnd);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRtdxFFTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRtdxFFTDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

UINT CRtdxFFTDlg::RTDXThreadFunction(LPVOID lpVoid)
{
	CMatlabClass* pMatlab;				// Matlab class variable
	CRtdxExp *pRtdx;					// RTDX class variable
	VARIANT RtdxBuffer;					// Variant variable to hold array from RTDX

	// Array embeded in the variant data type
	SAFEARRAYBOUND SafeArrayBound [] = {RTDX_BUFFER_SIZE, 0};

	float* pBuffer;

	// Initialise COM
	AfxOleInit();

	VariantInit(&RtdxBuffer);			// initialise variant variable
	RtdxBuffer.vt = VT_ARRAY | VT_R4;	// embed an array of type float in the variant variable

	// create the variant variable with the embeded float array
	RtdxBuffer.parray = SafeArrayCreate(VT_R4, 1, SafeArrayBound);

	pMatlab = new CMatlabClass;			// instantiate the Matlab class
	pMatlab->OpenMatlab(NULL);			// open Matlab
	pMatlab->ShowMatlab(FALSE);			// make Matlab invisible

	pRtdx = new CRtdxExp;				// instantiate the RTDX class using the COM interface "RTDX"
	pRtdx->CreateDispatch(_T("RTDX"));

	// set a processor type
	pRtdx->SetProcessor(_T("C6713 DSK"), _T("CPU_1"));

	pRtdx->Open("ochan", "R");			// open an RTDX channel for reading
	pRtdx->EnableChannel(_T("ochan"));	// enable the channel

	// create a Matlab vector array to hold an array of floats
	mxArray *pMatlabBuffer;
	pMatlabBuffer = mxCreateNumericMatrix(RTDX_BUFFER_SIZE, 1, mxSINGLE_CLASS, mxREAL);

	// turn on double buffering in Matlab to that the plots do not flicker
	pMatlab->ExecuteLine(_T("set(gcf,'DoubleBuffer','on');"));

	// define the sample frequency
	pMatlab->ExecuteLine(_T("fs = 16e3;"));

	// define the length of the FFT
	pMatlab->ExecuteLine(_T("fftlen = 256;"));

	// create an array for the frequency step
	pMatlab->ExecuteLine(_T("fp = [0: fs/fftlen: fs*(1/2 - 1/fftlen)];"));

	// loop until the main thread signals a termination by setting nFlag
	while(nFlag)
	{
		// read an array of N bytes from RTDX
		pRtdx->Read(&RtdxBuffer, VT_R4, sizeof(float)*RTDX_BUFFER_SIZE);
		SafeArrayAccessData(RtdxBuffer.parray, (void**)&pBuffer);

		// copy array read from RTDX to Matlab
		CopyMemory(mxGetPr(pMatlabBuffer), pBuffer, sizeof(float)*RTDX_BUFFER_SIZE);
		pMatlab->PutVariable("buffer", pMatlabBuffer);

		// release the RTDX array
		SafeArrayUnaccessData(RtdxBuffer.parray);

		// convert the float array received into a double
		pMatlab->ExecuteLine(_T("fftMag = double(buffer);"));

		// plot using Matlab plot command
		pMatlab->ExecuteLine(_T("plot(fp, fftMag(1:128))"));
	};

	// destroy Matlab vector array
	mxDestroyArray(pMatlabBuffer);

	// clear the variant variable
    VariantClear(&RtdxBuffer);

	// close RTDX and Matlab
	pRtdx->Close();
	pMatlab->CloseMatlab();

	// delete the instants of RTDX and Matlab
	delete pRtdx;
	delete pMatlab;

	return 0;
}

void CRtdxFFTDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	nFlag = 0;
	WaitForSingleObject(pRTDXThread->m_hThread, INFINITE);

}

