#ifndef __TARGET_H
#define __TARGET_H

/***********************************************************************
* FILENAME: $Source: /db/sds/syseng/rtdx/target/examples/c6x/target.h,v $
* VERSION : $Revision: 1.4 $
* DATE    : $Date: 1998/12/30 22:18:28 $
* Copyright (c) 1997 Texas Instruments Incorporated
*
* - Target specific initialization details
*
************************************************************************/

extern cregister volatile unsigned int IER;
extern cregister volatile unsigned int ISR;
extern cregister volatile unsigned int CSR;

#define NMIE_BIT	0x00000002
#define TARGET_INITIALIZE() \
	IER |= 0x00000001 | NMIE_BIT; \
	CSR |= 0x00000001;

#endif // __TARGET_H
