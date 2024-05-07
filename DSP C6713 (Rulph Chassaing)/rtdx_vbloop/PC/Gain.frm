VERSION 5.00
Begin VB.Form Loop_Gain 
   Caption         =   "Loop Gain"
   ClientHeight    =   2745
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   4830
   Icon            =   "Gain.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   2745
   ScaleWidth      =   4830
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Send_gain 
      Caption         =   "Send To DSK"
      Height          =   615
      Left            =   1800
      TabIndex        =   2
      Top             =   1920
      Width           =   1095
   End
   Begin VB.TextBox Gain_factor 
      Height          =   375
      Left            =   2160
      TabIndex        =   0
      Top             =   1080
      Width           =   375
   End
   Begin VB.Label Label1 
      Caption         =   "Enter Gain factor for Loop Program Range 1-5"
      Height          =   495
      Left            =   1080
      TabIndex        =   1
      Top             =   240
      Width           =   2655
   End
End
Attribute VB_Name = "Loop_Gain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

                                            'RTDX OLE API Status Return codes
Const SUCCESS = &H0                         'Method call succussful
Const FAIL = &H80004005                     'Method call failure
Const ENoDataAvailable = &H8003001E         'No data is currently available
Const EEndOfLogFile = &H80030002            'End of log file

Dim rtdx As Object


Private Sub Form_Load()
Dim status As Long
    
    BoardProc_frm.Show 1

    ' Get application objects
    Set rtdx = CreateObject("RTDX")
    
    status = rtdx.SetProcessor(BoardProc_frm.CurrentSelectedBoard, _
                                BoardProc_frm.CurrentSelectedProcessor)
    If (status <> SUCCESS) Then
        MsgBox "Set Processor failed", vbCritical
        Exit Sub
    End If
                        
    ' open target's input channel
    status = rtdx.Open("control_channel", "W")
    Select Case status
    Case Is = SUCCESS
    Case Is = FAIL
        MsgBox "Unable to open control_channel", vbCritical, "Error"
        Exit Sub
    Case Else
        MsgBox "Unknown return value from control_channel open", vbInformation
        Exit Sub
    End Select
End Sub

Private Sub Form_Unload(Cancel As Integer)
    ' close target's input channel
    status = rtdx.Close()
    Select Case status
        Case Is = SUCCESS
        Case Is = FAIL
            MsgBox "Unable to close input channel", vbCritical, "Error"
        Case Else
            MsgBox "Unknown return value from input close", vbInformation
    End Select
  
    Set rtdx = Nothing                     ' kill RTDX OLE object
    Unload BoardProc_frm
End Sub

Private Sub Send_gain_Click()
    Dim status As Long
    Dim dataI4 As Long
    Dim bufstate As Long
    ' write gain to target input channel
    dataI4 = Gain_factor.Text
    status = rtdx.WriteI4(dataI4, bufstate)
    
    Select Case status
        Case Is = SUCCESS
        Case Is = FAIL
            MsgBox "Error: Writing data failed" & dataI4
        Case Else
            MsgBox "Gain data backed up waiting for DSP to read it"
    End Select
End Sub
