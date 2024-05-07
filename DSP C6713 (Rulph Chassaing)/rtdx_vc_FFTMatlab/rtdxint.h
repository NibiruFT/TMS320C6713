// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// IRtdxExp wrapper class

class IRtdxExp : public COleDispatchDriver
{
public:
	IRtdxExp() {}		// Calls COleDispatchDriver default constructor
	IRtdxExp(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IRtdxExp(const IRtdxExp& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long Open(LPCTSTR Channel_String, LPCTSTR Read_Write);
	long Close();
	long Read(VARIANT* pArr, long dataType, long numBytes);
	// method 'ReadI1' not emitted because of invalid return type or parameter type
	long ReadI2(short* pData);
	long ReadI4(long* pData);
	long ReadF4(float* pData);
	long ReadF8(double* pData);
	long ReadSAI1(VARIANT* pArr);
	long ReadSAI2(VARIANT* pArr);
	long ReadSAI4(VARIANT* pArr);
	long ReadSAF4(VARIANT* pArr);
	long ReadSAF8(VARIANT* pArr);
	VARIANT ReadSAI2V(long* pStatus);
	VARIANT ReadSAI4V(long* pStatus);
	// method 'WriteI1' not emitted because of invalid return type or parameter type
	long WriteI2(short Data, long* numBytes);
	long WriteI4(long Data, long* numBytes);
	long WriteF4(float Data, long* numBytes);
	long WriteF8(double Data, long* numBytes);
	long Write(const VARIANT& Arr, long* numBytes);
	long Rewind();
	long Flush();
	long Seek(long MsgNum);
	long SeekData(long numBytes);
	long StatusOfWrite(long* numBytes);
	long GetNumMsgs(long* pNum);
	long GetChannelID(LPCTSTR Channel_String, long* chanId);
	long GotoNextMsg();
	long GetMsgID(long* pMsgId);
	long GetMsgNumber(long* pMsgNum);
	long GetMsgLength(long* pLength);
	long EnableRtdx();
	long DisableRtdx();
	long EnableChannel(LPCTSTR ChannelName);
	long DisableChannel(LPCTSTR ChannelName);
	long GetChannelStatus(LPCTSTR ChannelName, long* pChannelStatus);
	long ConfigureRtdx(short Mode, long MainBufferSize, long NumOfMainBuffers);
	long ConfigureLogFile(LPCTSTR FileName, long FileSize, short FileFullMode, short FileOpenMode);
	long GetRTDXRev(long* RevNum);
	long GetStatusString(BSTR* StatusString);
	long GetCapability(long* Capability);
	long RunDiagnostics(short TestType, long TestMode, long TestInfo);
	CString GetDiagFilePath(short TestType);
	long SetProcessor(LPCTSTR Board, LPCTSTR Cpu);
};
