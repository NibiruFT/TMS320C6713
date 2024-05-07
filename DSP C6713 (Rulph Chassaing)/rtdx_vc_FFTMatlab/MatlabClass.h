// MatlabClass.h: interface for the CMatlabClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATLABCLASS_H__DE60BDAF_7784_11D8_8735_000347A3D902__INCLUDED_)
#define AFX_MATLABCLASS_H__DE60BDAF_7784_11D8_8735_000347A3D902__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Engine.h"
#pragma comment(lib, "libeng.lib")
#pragma comment(lib, "libmx.lib")

class CMatlabClass  
{
public:
	Engine*  pEngine;

void OpenMatlab(LPCTSTR lpCommand);
int CloseMatlab(void);
int ExecuteLine(LPCTSTR lpExpression);
mxArray* GetVariable(LPCTSTR lpVariableName);
int PutVariable(LPCTSTR lpVariableName, const mxArray* pMatlabArray);
int ShowMatlab(bool bShow);
int CreateBuffer(char* pOutputBuffer, int nLength);

	CMatlabClass();
	virtual ~CMatlabClass();

};

#endif // !defined(AFX_MATLABCLASS_H__DE60BDAF_7784_11D8_8735_000347A3D902__INCLUDED_)
