; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CRTDXThread
LastTemplate=CWinThread
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "RtdxFFT.h"

ClassCount=3
Class1=CRtdxFFTApp
Class2=CRtdxFFTDlg

ResourceCount=2
Resource1=IDR_MAINFRAME
Class3=CRTDXThread
Resource2=IDD_RTDXFFT_DIALOG

[CLS:CRtdxFFTApp]
Type=0
HeaderFile=RtdxFFT.h
ImplementationFile=RtdxFFT.cpp
Filter=N

[CLS:CRtdxFFTDlg]
Type=0
HeaderFile=RtdxFFTDlg.h
ImplementationFile=RtdxFFTDlg.cpp
Filter=D
LastObject=CRtdxFFTDlg
BaseClass=CDialog
VirtualFilter=dWC



[DLG:IDD_RTDXFFT_DIALOG]
Type=1
Class=?
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342177795

[CLS:CRTDXThread]
Type=0
HeaderFile=RTDXThread.h
ImplementationFile=RTDXThread.cpp
BaseClass=CWinThread
Filter=N

