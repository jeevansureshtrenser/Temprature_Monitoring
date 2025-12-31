/************************************************************
* File name     : polling.h
* Description   : header file for polling
* Author        : Jeevan Suresh
* License       : 
**************************************************************/
#ifndef __POLLING_H__
#define __POLLING_H_
#include "../include/common.h"
/******************************Macro Definitions************************************/
#define POLLING_THREAD_ID   1

/******************************Local Variable Declaration******************************/
typedef struct
{
    PARAMETER_TYPE param_t;
    ERROR_TYPE (*iReadfn)(int *);
    long int wiPollingTime;
    long int wiLastPollingTime;

}PollingThreadConfig;

/******************************Function Declaration******************************/
extern void* wvdPollingThread(void *arg);

#endif /*__POLLING_H_*/
