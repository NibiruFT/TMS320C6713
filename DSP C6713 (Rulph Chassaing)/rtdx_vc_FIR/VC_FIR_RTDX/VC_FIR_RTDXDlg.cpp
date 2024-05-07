// VC_FIR_RTDXDlg.cpp : implementation file
//
#include "stdafx.h"
#include "VC_FIR_RTDX.h"
#include "VC_FIR_RTDXDlg.h"

#include <fstream>
#include <iomanip>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RTDX_BUFFER_SIZE 256
using namespace std;

// global variables
VARIANT RtdxBuffer;
SAFEARRAYBOUND SafeArrayBound [] = {RTDX_BUFFER_SIZE, 0};
CString pathName;
short* pBuffer;
int i;
long NumberOfBytes;
char szFilters[] = "FIR Coefficient Files (*.cof)|*.cof|All Files (*.*)|*.*||";

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVC_FIR_RTDXDlg dialog

CVC_FIR_RTDXDlg::CVC_FIR_RTDXDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVC_FIR_RTDXDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVC_FIR_RTDXDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVC_FIR_RTDXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVC_FIR_RTDXDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVC_FIR_RTDXDlg, CDialog)
	//{{AFX_MSG_MAP(CVC_FIR_RTDXDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOAD_FILE, OnLoadFile)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVC_FIR_RTDXDlg message handlers

BOOL CVC_FIR_RTDXDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Initialize COM
	AfxOleInit();
	
	pRTDX = new IRtdxExp;
	pRTDX->CreateDispatch(_T("RTDX"));

	
	if(pRTDX->SetProcessor(_T("C6713 DSK"), _T("CPU_1")))
	{
		MessageBox("Could not set the processor!", "Error");
		return FALSE;
	}
	

	if(pRTDX->Open("control_channel", "W"))
	{
		MessageBox("Could not open the channel!", "Error");
		return FALSE;
	}

	// create the variant array to hold the FIR coefficients
	VariantInit(&RtdxBuffer);
	RtdxBuffer.vt = VT_ARRAY | VT_I2;
	RtdxBuffer.parray = SafeArrayCreate(VT_I2, 1, SafeArrayBound);
	SafeArrayAccessData(RtdxBuffer.parray, (void**)&pBuffer);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVC_FIR_RTDXDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVC_FIR_RTDXDlg::OnPaint() 
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
HCURSOR CVC_FIR_RTDXDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CVC_FIR_RTDXDlg::OnLoadFile() 
{
	// Create an Open dialog; the default file name extension is ".cof".
	CFileDialog fileDlg (TRUE, "cof", "*.cof",
      OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);
   
	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 
	// returns IDOK.
	if(fileDlg.DoModal() == IDOK)
	{
		pathName = fileDlg.GetPathName();
		ifstream filestream((LPCTSTR)pathName);

		if(filestream.good())
		{
			filestream >> pBuffer[0];
			
			for(i = 1; i < pBuffer[0]; i++)
				filestream >> pBuffer[i];
			
			filestream.close();

			pRTDX->Write(RtdxBuffer, &NumberOfBytes);
		}
   
	}
}

void CVC_FIR_RTDXDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(pRTDX->Close())
		MessageBox("Could not close the channel!", "Error");

	SafeArrayUnaccessData(RtdxBuffer.parray);
	VariantClear(&RtdxBuffer);

	delete pRTDX;
}
