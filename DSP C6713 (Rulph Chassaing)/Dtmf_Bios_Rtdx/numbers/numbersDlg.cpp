// numbersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "numbers.h"
#include "numbersDlg.h"
#include "rtdxint.h"

#define WM_READ_NUMBER (WM_APP + 1)
#define length 15
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int nFlag = 1;
int nRTDX;
int array[length];
int counter=0,n,gotnewnumber,zerobefore=0;
int weightzero,weight1,weight2,weight3,weight4,weight5,weight6,weight7,weight8,weight9,
	weightstar,weight0,weightpound;

/////////////////////////////////////////////////////////////////////////////
// CNumbersDlg dialog

CNumbersDlg::CNumbersDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNumbersDlg::IDD, pParent)
{
	pRtdxThread = NULL;

	//{{AFX_DATA_INIT(CNumbersDlg)
	m_value = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNumbersDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNumbersDlg)
	DDX_Text(pDX, IDC_EDIT1, m_value);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNumbersDlg, CDialog)
	ON_MESSAGE(WM_READ_NUMBER, ProcessRtdx)
	//{{AFX_MSG_MAP(CNumbersDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnClear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNumbersDlg message handlers

BOOL CNumbersDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	pRtdxThread = (CRtdxThread*)AfxBeginThread(RtdxPolling, m_hWnd);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNumbersDlg::OnPaint() 
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
HCURSOR CNumbersDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

UINT CNumbersDlg::RtdxPolling(LPVOID lpvoid)
{

	IRtdxExp *pRtdx;
	short nValue;
	HWND hWnd;

	AfxOleInit();
    
	pRtdx = new IRtdxExp;
	pRtdx->CreateDispatch(_T("RTDX"));
	pRtdx->SetProcessor(_T("C6713 DSK"), _T("CPU_1"));
	pRtdx->Open("ochan", "R");
	pRtdx->EnableChannel(_T("ochan"));

	hWnd = (HWND)lpvoid;

	while(nFlag)
	{
		pRtdx->ReadI2((short*)&nValue);

		// Added this new conditional statement to make sure the 
		// program does not send a WM_READ_NUMBER message before
		// the thread is killed.
		if(nFlag)
			::SendMessage(hWnd, WM_READ_NUMBER, nValue, 0);
	};

	pRtdx->Close();
	
	delete pRtdx;

	return 0;
}

void CNumbersDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	nFlag = 0;
	if(pRtdxThread != NULL)
		WaitForSingleObject(pRtdxThread->m_hThread, INFINITE);	
}

LRESULT CNumbersDlg::ProcessRtdx(WPARAM wParam, LPARAM lParam)
{

	nRTDX = (int)wParam;
	if(counter<length) counter++; else counter=0;
	array[counter]=nRTDX;

    weightzero=0; weight1=0; weight2=0; weight3=0; weight4=0; weight5=0; weight6=0; 
    weight7=0; weight8=0; weight9=0; weightstar=0; weight0=0; weightpound=0; 
    for(n = 0; n < length; n++)
    {
     if(array[n]==1) weight1++;
	 if(array[n]==2) weight2++;
	 if(array[n]==3) weight3++;
	 if(array[n]==4) weight4++;
	 if(array[n]==5) weight5++;
	 if(array[n]==6) weight6++;
	 if(array[n]==7) weight7++;
	 if(array[n]==8) weight8++;
	 if(array[n]==9) weight9++;
	 if(array[n]==10) weightstar++;
	 if(array[n]==11) weight0++;
	 if(array[n]==12) weightpound++;
	 if((array[n]!=1)&&(array[n]!=2)&&(array[n]!=3)&&(array[n]!=4)&&(array[n]!=5)
      &&(array[n]!=6)&&(array[n]!=7)&&(array[n]!=8)&&(array[n]!=9)&&(array[n]!=10)
	  &&(array[n]!=11)&&(array[n]!=12)) weightzero++;
	}
	gotnewnumber=0;
	if((weight1>=(0.7*length))&&(zerobefore==1))     {gotnewnumber=1; zerobefore=0; m_RTDX="1";}
	if((weight2>=(0.7*length))&&(zerobefore==1))     {gotnewnumber=1; zerobefore=0; m_RTDX="2";}
	if((weight3>=(0.7*length))&&(zerobefore==1))     {gotnewnumber=1; zerobefore=0; m_RTDX="3";}
	if((weight4>=(0.7*length))&&(zerobefore==1))     {gotnewnumber=1; zerobefore=0; m_RTDX="4";}
	if((weight5>=(0.7*length))&&(zerobefore==1))     {gotnewnumber=1; zerobefore=0; m_RTDX="5";}
	if((weight6>=(0.7*length))&&(zerobefore==1))     {gotnewnumber=1; zerobefore=0; m_RTDX="6";}
	if((weight7>=(0.7*length))&&(zerobefore==1))     {gotnewnumber=1; zerobefore=0; m_RTDX="7";}
	if((weight8>=(0.7*length))&&(zerobefore==1))     {gotnewnumber=1; zerobefore=0; m_RTDX="8";}
	if((weight9>=(0.7*length))&&(zerobefore==1))     {gotnewnumber=1; zerobefore=0; m_RTDX="9";}
	if((weightstar>=(0.7*length))&&(zerobefore==1))  {gotnewnumber=1; zerobefore=0; m_RTDX="*";}
	if((weight0>=(0.7*length))&&(zerobefore==1))     {gotnewnumber=1; zerobefore=0; m_RTDX="0";}
	if((weightpound>=(0.7*length))&&(zerobefore==1)) {gotnewnumber=1; zerobefore=0; m_RTDX="#";}
	if(weightzero>=(0.7*length)) zerobefore=1;

	if(gotnewnumber==1)
	{
		m_value = m_value + m_RTDX;
		UpdateData(FALSE);
	}

	return 0;
}

void CNumbersDlg::OnClear() 
{
	// TODO: Add your control notification handler code here
	m_value = "";
	UpdateData(FALSE);
}
