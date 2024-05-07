// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "rtdxint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CRtdxExp properties

/////////////////////////////////////////////////////////////////////////////
// CRtdxExp operations

long CRtdxExp::Open(LPCTSTR Channel_String, LPCTSTR Read_Write)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Channel_String, Read_Write);
	return result;
}

long CRtdxExp::Close()
{
	long result;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CRtdxExp::Read(VARIANT* pArr, long dataType, long numBytes)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_I4 VTS_I4;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		pArr, dataType, numBytes);
	return result;
}

long CRtdxExp::ReadI2(short* pData)
{
	long result;
	static BYTE parms[] =
		VTS_PI2;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		pData);
	return result;
}

long CRtdxExp::ReadI4(long* pData)
{
	long result;
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		pData);
	return result;
}

long CRtdxExp::ReadF4(float* pData)
{
	long result;
	static BYTE parms[] =
		VTS_PR4;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		pData);
	return result;
}

long CRtdxExp::ReadF8(double* pData)
{
	long result;
	static BYTE parms[] =
		VTS_PR8;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		pData);
	return result;
}

long CRtdxExp::ReadSAI1(VARIANT* pArr)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		pArr);
	return result;
}

long CRtdxExp::ReadSAI2(VARIANT* pArr)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		pArr);
	return result;
}

long CRtdxExp::ReadSAI4(VARIANT* pArr)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		pArr);
	return result;
}

long CRtdxExp::ReadSAF4(VARIANT* pArr)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		pArr);
	return result;
}

long CRtdxExp::ReadSAF8(VARIANT* pArr)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		pArr);
	return result;
}

VARIANT CRtdxExp::ReadSAI2V(long* pStatus)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		pStatus);
	return result;
}

VARIANT CRtdxExp::ReadSAI4V(long* pStatus)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		pStatus);
	return result;
}

long CRtdxExp::WriteI2(short Data, long* numBytes)
{
	long result;
	static BYTE parms[] =
		VTS_I2 VTS_PI4;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Data, numBytes);
	return result;
}

long CRtdxExp::WriteI4(long Data, long* numBytes)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Data, numBytes);
	return result;
}

long CRtdxExp::WriteF4(float Data, long* numBytes)
{
	long result;
	static BYTE parms[] =
		VTS_R4 VTS_PI4;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Data, numBytes);
	return result;
}

long CRtdxExp::WriteF8(double Data, long* numBytes)
{
	long result;
	static BYTE parms[] =
		VTS_R8 VTS_PI4;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Data, numBytes);
	return result;
}

long CRtdxExp::Write(const VARIANT& Arr, long* numBytes)
{
	long result;
	static BYTE parms[] =
		VTS_VARIANT VTS_PI4;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		&Arr, numBytes);
	return result;
}

long CRtdxExp::Rewind()
{
	long result;
	InvokeHelper(0x16, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CRtdxExp::Flush()
{
	long result;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CRtdxExp::Seek(long MsgNum)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		MsgNum);
	return result;
}

long CRtdxExp::SeekData(long numBytes)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x19, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		numBytes);
	return result;
}

long CRtdxExp::StatusOfWrite(long* numBytes)
{
	long result;
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		numBytes);
	return result;
}

long CRtdxExp::GetNumMsgs(long* pNum)
{
	long result;
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		pNum);
	return result;
}

long CRtdxExp::GetChannelID(LPCTSTR Channel_String, long* chanId)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_PI4;
	InvokeHelper(0x1c, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Channel_String, chanId);
	return result;
}

long CRtdxExp::GotoNextMsg()
{
	long result;
	InvokeHelper(0x1d, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CRtdxExp::GetMsgID(long* pMsgId)
{
	long result;
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x1e, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		pMsgId);
	return result;
}

long CRtdxExp::GetMsgNumber(long* pMsgNum)
{
	long result;
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		pMsgNum);
	return result;
}

long CRtdxExp::GetMsgLength(long* pLength)
{
	long result;
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x20, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		pLength);
	return result;
}

long CRtdxExp::EnableRtdx()
{
	long result;
	InvokeHelper(0x21, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CRtdxExp::DisableRtdx()
{
	long result;
	InvokeHelper(0x22, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CRtdxExp::EnableChannel(LPCTSTR ChannelName)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x23, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		ChannelName);
	return result;
}

long CRtdxExp::DisableChannel(LPCTSTR ChannelName)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x24, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		ChannelName);
	return result;
}

long CRtdxExp::GetChannelStatus(LPCTSTR ChannelName, long* pChannelStatus)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_PI4;
	InvokeHelper(0x25, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		ChannelName, pChannelStatus);
	return result;
}

long CRtdxExp::ConfigureRtdx(short Mode, long MainBufferSize, long NumOfMainBuffers)
{
	long result;
	static BYTE parms[] =
		VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x26, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Mode, MainBufferSize, NumOfMainBuffers);
	return result;
}

long CRtdxExp::ConfigureLogFile(LPCTSTR FileName, long FileSize, short FileFullMode, short FileOpenMode)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I2 VTS_I2;
	InvokeHelper(0x27, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		FileName, FileSize, FileFullMode, FileOpenMode);
	return result;
}

long CRtdxExp::GetRTDXRev(long* RevNum)
{
	long result;
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x28, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		RevNum);
	return result;
}

long CRtdxExp::GetStatusString(BSTR* StatusString)
{
	long result;
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x29, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		StatusString);
	return result;
}

long CRtdxExp::GetCapability(long* Capability)
{
	long result;
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x2a, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Capability);
	return result;
}

long CRtdxExp::RunDiagnostics(short TestType, long TestMode, long TestInfo)
{
	long result;
	static BYTE parms[] =
		VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x2b, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		TestType, TestMode, TestInfo);
	return result;
}

CString CRtdxExp::GetDiagFilePath(short TestType)
{
	CString result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x2c, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		TestType);
	return result;
}

long CRtdxExp::SetProcessor(LPCTSTR Board, LPCTSTR Cpu)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x2d, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Board, Cpu);
	return result;
}
