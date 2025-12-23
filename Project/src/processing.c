/************************************************************
* File name     : processing.c
* Description   : source code for processing thread
* Author        : Jeevan Suresh
* License       : Copyright (c) 2025 Trenser 
                    All Rights Reserved
**************************************************************/
/***********************Include Files******************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/common.h"
#include "../include/processing.h"
#include "../include/database.h"

/***********************Function Declaration******************/
static ProcessThreadConfig wstProcessThreadConfig[] = 
{
    {   
        PARAM_TEMP,
        LOWER_TEMP_THRESHOLD,
        UPPER_TEMP_THRESHOLD,
        TEMP_LOWER_OPERATING_RANGE,
        TEMP_UPPER_OPERATING_RANGE,
        CHECKING_TEMP_INTERVAL,
        0
    },                                                  
    {   
        PARAM_PRESSURE,
        LOWER_PRSURE_THRESHOLD,
        UPPER_PRSURE_THRESHOLD,
        PRESSURE_LOWER_OPERATING_RANGE,
        PRESSURE_UPPER_OPERATING_RANGE,
        CHECKING_PRSSR_INTERVAL,
        0
    }
};
/***********************Function Declaration******************/
void* wvdProcessingThread(void *arg);


/******************************************************************************
* Function name     :   printinfo
* Description       :   prepare the information in to printable
                        form
* Arguments         :   WARNING_TYPE warningType_t - type of warning
                        CommonDatabase *astCommonDatabase - hold read value
                        const char* message - sensor warnings
* Return type       :   void
********************************************************************************/
void printinfo(WARNING_TYPE warningType_t, CommonDatabase *astCommonDatabase, const char* message)
{
    char *cMessage = (char*)malloc(256);
    if(cMessage != NULL)
    {
        if(astCommonDatabase != NULL)
        {
            if(message != NULL)
            {
                memset(cMessage, DEF_CLEAR, 256);
                switch(astCommonDatabase->param_t)
                {
                    case PARAM_TEMP     : snprintf(cMessage,256, "TEMPERATURE | %3d Degrees Celsius | %s ", astCommonDatabase->iReadVal, message);
                        break;
                    case PARAM_PRESSURE : snprintf(cMessage,256, "PRESSURE    | %3d PSI             | %s ", astCommonDatabase->iReadVal, message);
                        break;
                    default             : /* No process */
                        break;
                }
                printMessage(warningType_t,(const char*)cMessage);

            }
            else
            {
                /* No process*/
            }
        }
        else
        {
            /* No process*/
        }
    }
    else
    {
        /* No process*/
    }
    free(cMessage);
}
/******************************************************************************
* Function name     :   Check_out_of_Oprtng_range
* Description       :   check sensor value out of operating range
* Arguments         :   CommonDatabase *astCommonDatabase - hold read value
                        int aiPrcsConfigCount - index of global structure ProcessThreadConfig
* Return type       :   ERROR_TYPE - return errotype, if 0 NO_ERROR
********************************************************************************/
ERROR_TYPE Check_out_of_Oprtng_range(CommonDatabase *astCommonDatabase, int aiPrcsConfigCount)
{
    ERROR_TYPE errVal = NO_ERR;
    if(astCommonDatabase->iReadVal > wstProcessThreadConfig[aiPrcsConfigCount].wiUprOprtngRange 
        || astCommonDatabase->iReadVal < wstProcessThreadConfig[aiPrcsConfigCount].wiLwrOprtngRange)
    {
        errVal = ERRINVALID;
    }
    return errVal;
}
/******************************************************************************
* Function name     :   Check_out_of_Thrshld_range
* Description       :   check sensor value out of threshold range
* Arguments         :   CommonDatabase *astCommonDatabase - hold read value
                        int aiPrcsConfigCount - index of global structure ProcessThreadConfig
* Return type       :   ERROR_TYPE - return errotype, if 0 NO_ERROR
********************************************************************************/
ERROR_TYPE Check_out_of_Thrshld_range(CommonDatabase *astCommonDatabase, int aiPrcsConfigCount)
{
    ERROR_TYPE errVal = NO_ERR;
    if(astCommonDatabase->iReadVal > wstProcessThreadConfig[aiPrcsConfigCount].wiUpperThreshold 
        || astCommonDatabase->iReadVal < wstProcessThreadConfig[aiPrcsConfigCount].wiLowerThreshold)
    {
        errVal = ERRINVALID;
    }
    return errVal;
}
/*********************************************************************************
* Function name     : wvdPollingThread
* Description       : thread for polling the temperature and 
*                     pressure value periodically
* Arguments         : void *arg - thread id
* Return type       : void
***********************************************************************************/
ERROR_TYPE wubProcessData(CommonDatabase *astCommonDatabase, int aiPrcsConfigCount)
{
    ERROR_TYPE errVal = NO_ERR;
    if(astCommonDatabase == NULL)
    {
        return ERRINVALID;
    }
    if(Check_out_of_Oprtng_range(astCommonDatabase, aiPrcsConfigCount) == NO_ERR)
    {
        if(Check_out_of_Thrshld_range(astCommonDatabase, aiPrcsConfigCount) == NO_ERR)
        {
            if( g_ubDebugMode != DEF_CLEAR)
            {
                printinfo(DEBUG, astCommonDatabase, " ");
            }
            else
            {
                /* No process*/
            }

        }
        else
        {
            printinfo(WARNING, astCommonDatabase, "out of threshold range!");
        }
    }
    else
    {
        errVal = ERRINVALID;
        printinfo(WARNING, astCommonDatabase, "Sensor read Over bound!");
    }
    return errVal;
}
/************************************************************
* Function name     : wvdPollingThread
* Description       : thread for polling the temperature and 
*                     pressure value periodically
* Arguments         : void *arg - thread id
* Return type       : void
**************************************************************/
void* wvdProcessingThread(void *arg)
{
    int thread_id = *((int*)arg);

    clock_t lCurrent_processtime     = DEF_CLEAR;
    clock_t lCurrent_processtime_sec = DEF_CLEAR;
    ERROR_TYPE  err_t                = NO_ERR;
    int     aiProcessConfgSize       = DEF_CLEAR;
    int aiPrcsConfigCount            = DEF_CLEAR;

    CommonDatabase astCommonDatabase;

    aiProcessConfgSize = sizeof(wstProcessThreadConfig) / sizeof(wstProcessThreadConfig[0]);
    while(1)
    {
    
        for(aiPrcsConfigCount = 0; aiPrcsConfigCount < aiProcessConfgSize; aiPrcsConfigCount++)
        {
            lCurrent_processtime = clock();
            lCurrent_processtime_sec = (lCurrent_processtime) / CLOCKS_PER_SEC;

            if((lCurrent_processtime_sec - wstProcessThreadConfig[aiPrcsConfigCount].wiLastCheckTime)  
                >= wstProcessThreadConfig[aiPrcsConfigCount].wiCheckingTime)
            {
                pthread_mutex_lock(&g_pthreadlock);
                err_t = wstRetrive_data_node(wstProcessThreadConfig[aiPrcsConfigCount].param_t, &astCommonDatabase);
                pthread_mutex_unlock(&g_pthreadlock);
                if(err_t == NO_ERR)
                {
                    err_t = wubProcessData(&astCommonDatabase, aiPrcsConfigCount);
                    if(err_t == NO_ERR)
                   {
                        wstProcessThreadConfig[aiPrcsConfigCount].wiLastCheckTime = lCurrent_processtime_sec;
                   }
                    
                }
                else
                {
                    /* No Process*/
                }
            }
            else
            {
                 /* No Process*/
            }

        }
    }
    printf(" exit thread : %d", thread_id);
}

/* end of file*/
