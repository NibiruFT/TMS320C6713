// MatlabClass.cpp: implementation of the CMatlabClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "vc_spectrogram.h"
#include "MatlabClass.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMatlabClass::CMatlabClass()
: pEngine(NULL)
{

}

CMatlabClass::~CMatlabClass()
{

}

void CMatlabClass::OpenMatlab(LPCTSTR lpCommand)
{
	pEngine = engOpen(lpCommand);
}

int CMatlabClass::CloseMatlab(void)
{
	if(pEngine)
		engClose(pEngine);
	else
		pEngine = NULL;

	return 0;
}

int CMatlabClass::ExecuteLine(LPCTSTR lpExpression)
{
	return engEvalString(pEngine, lpExpression);
}

mxArray* CMatlabClass::GetVariable(LPCTSTR lpVariableName)
{
	return engGetVariable(pEngine, lpVariableName);
}

int CMatlabClass::PutVariable(LPCTSTR lpVariableName, const mxArray* pMatlabArray)
{
	return engPutVariable(pEngine, lpVariableName, pMatlabArray);
}

int CMatlabClass::ShowMatlab(bool bShow)
{
	return engSetVisible(pEngine, bShow);
}

int CMatlabClass::CreateBuffer(char* pOutputBuffer, int nLength)
{
	return engOutputBuffer(pEngine, pOutputBuffer, nLength);
}

