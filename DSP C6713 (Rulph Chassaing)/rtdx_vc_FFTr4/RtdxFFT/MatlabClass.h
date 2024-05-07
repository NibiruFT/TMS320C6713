// MatlabClass.h: interface for the CMatlabClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATLABCLASS_H__93088A2E_28D2_433F_8B76_AD7B82E35641__INCLUDED_)
#define AFX_MATLABCLASS_H__93088A2E_28D2_433F_8B76_AD7B82E35641__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "Matlab.h"
#include "Engine.h"

// Required Matlab library for static linking 
#pragma comment(lib, "libeng.lib")
#pragma comment(lib, "libmx.lib")
//#pragma comment(lib, "libmatlb.lib")
//#pragma comment(lib, "libmat.lib")
//#pragma comment(lib, "libmmfile.lib")

class CMatlabClass  
{
public:
	CMatlabClass();
	virtual ~CMatlabClass();

protected:
	Engine* pEngine;

public:
	void OpenMatlab(LPCTSTR lpCommand);
	int CloseMatlab(void);
	int ExecuteLine(LPCTSTR lpExpression);
	mxArray* GetVariable(LPCTSTR lpVariableName);
	int PutVariable(LPCTSTR lpVariableName, const mxArray* pMatlabArray);
	int ShowMatlab(bool bShow);
	int CreateBuffer(char* pOutputBuffer, int nLength);

};

#endif // !defined(AFX_MATLABCLASS_H__93088A2E_28D2_433F_8B76_AD7B82E35641__INCLUDED_)
