/************************************************************
* File name     : processing.h
* Description   : header file for processing thread
* Author        : Jeevan Suresh
* License       : 
**************************************************************/
#ifndef __PROCESSING_H__
#define __PROCESSING_H__

/******************************Macro Definition****************************************/
#define PROCESS_THREAD_ID   2

/******************************Local Variable Declaration******************************/
typedef struct
{
    PARAMETER_TYPE param_t;
    int wiLowerThreshold;
    int wiUpperThreshold;
    int wiLwrOprtngRange;
    int wiUprOprtngRange;
    long int wiCheckingTime;
    long int wiLastCheckTime;
}ProcessThreadConfig;
/******************************Function Declaration******************************/
extern void* wvdProcessingThread( void *arg );

#endif /*__PROCESSING_H__*/
