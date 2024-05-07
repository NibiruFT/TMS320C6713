#ifndef __COMMON_H
#define __COMMON_H

/***********************************************************************
* FILENAME: $RCSfile: common.h,v $
* VERSION : $Revision: 1.5 $
* DATE    : $Date: 1998/08/24 17:11:14 $
* Copyright (c) 1997 Texas Instruments Incorporated
*
* - common routines used throughout tests
*
************************************************************************/

#if RTDX_POLLING_IMPLEMENTATION
#define WAIT_FOR_XFER()		while( RTDX_writing != NULL ) RTDX_Poll()
#else
#define WAIT_FOR_XFER()		while( RTDX_writing != NULL )
#endif

#endif // __COMMON_H
