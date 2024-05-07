; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CRTDXThread
LastTemplate=CWinThread
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "rtdx_vc_FFTMatlab.h"

ClassCount=4
Class1=CRtdx_vc_FFTMatlabApp
Class2=CRtdx_vc_FFTMatlabDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CRTDXThread
Resource3=IDD_RTDX_VC_FFTMATLAB_DIALOG

[CLS:CRtdx_vc_FFTMatlabApp]
Type=0
HeaderFile=rtdx_vc_FFTMatlab.h
ImplementationFile=rtdx_vc_FFTMatlab.cpp
Filter=N

[CLS:CRtdx_vc_FFTMatlabDlg]
Type=0
HeaderFile=rtdx_vc_FFTMatlabDlg.h
ImplementationFile=rtdx_vc_FFTMatlabDlg.cpp
Filter=D
LastObject=CRtdx_vc_FFTMatlabDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=rtdx_vc_FFTMatlabDlg.h
ImplementationFile=rtdx_vc_FFTMatlabDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_RTDX_VC_FFTMATLAB_DIALOG]
Type=1
Class=CRtdx_vc_FFTMatlabDlg
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352

[CLS:CRTDXThread]
Type=0
HeaderFile=RTDXThread.h
ImplementationFile=RTDXThread.cpp
BaseClass=CWinThread
Filter=N
LastObject=CRTDXThread

