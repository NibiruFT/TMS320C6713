; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CNumbersDlg
LastTemplate=CWinThread
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "numbers.h"

ClassCount=3
Class1=CNumbersApp
Class2=CNumbersDlg

ResourceCount=2
Resource1=IDR_MAINFRAME
Class3=CRtdxThread
Resource2=IDD_NUMBERS_DIALOG

[CLS:CNumbersApp]
Type=0
HeaderFile=numbers.h
ImplementationFile=numbers.cpp
Filter=N

[CLS:CNumbersDlg]
Type=0
HeaderFile=numbersDlg.h
ImplementationFile=numbersDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_BUTTON1



[DLG:IDD_NUMBERS_DIALOG]
Type=1
Class=CNumbersDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308353
Control3=IDC_STATIC,static,1342177795
Control4=IDC_EDIT1,edit,1350633600
Control5=IDC_BUTTON1,button,1342242816

[CLS:CRtdxThread]
Type=0
HeaderFile=RtdxThread.h
ImplementationFile=RtdxThread.cpp
BaseClass=CWinThread
Filter=N

