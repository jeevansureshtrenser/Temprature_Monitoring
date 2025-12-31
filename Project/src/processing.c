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
#include <time.h>
#include "../include/common.h"
#include "../include/processing.h"
#include "../include/database.h"

/***********************variable Declaration******************/
static const size_t MESSAGE_BUFFER_SIZE = 256U;
static const int FORMAT_WIDTH = 3;
/***********************Global Variable Declaration******************/  
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
* Function name     :   Printinfo
* Description       :   prepare the information in to printable
                        form
* Arguments         :   WARNING_TYPE warningType_t - type of warning
                        CommonDatabase *astCommonDatabase - hold read value
                        const char* message - sensor warnings
* Return type       :   void
********************************************************************************/
void Printinfo(WARNING_TYPE warningType_t, CommonDatabase *astCommonDatabase, const char* message)
{
    char *cMessage = (char*)malloc(MESSAGE_BUFFER_SIZE);
    if(cMessage != NULL)
    {
        if(astCommonDatabase != NULL)
        {
            if(message != NULL)
            {
                (void)memset(cMessage, DEF_CLEAR, MESSAGE_BUFFER_SIZE);
                switch(astCommonDatabase->param_t)
                {
                    case PARAM_TEMP     : (void)snprintf(cMessage,MESSAGE_BUFFER_SIZE, "TEMPERATURE | %*d Degrees Celsius | %s ", FORMAT_WIDTH, astCommonDatabase->iReadVal, message);
                        break;
                    case PARAM_PRESSURE : (void)snprintf(cMessage,MESSAGE_BUFFER_SIZE, "PRESSURE    | %*d PSI             | %s ", FORMAT_WIDTH, astCommonDatabase->iReadVal, message);
                        break;
                    default             : /* No process */
                        break;
                }
                PrintMessage(warningType_t,(const char*)cMessage);

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
* Function name     :   CheckOperatingRange
* Description       :   check sensor value out of operating range
* Arguments         :   CommonDatabase *astCommonDatabase - hold read value
                        int aiPrcsConfigCount - index of global structure ProcessThreadConfig
* Return type       :   ERROR_TYPE - return errotype, if 0 NO_ERROR
********************************************************************************/
ERROR_TYPE CheckOperatingRange(CommonDatabase *astCommonDatabase, int aiPrcsConfigCount)
{
    ERROR_TYPE errVal = NO_ERR;
    if(astCommonDatabase->iReadVal > wstProcessThreadConfig[aiPrcsConfigCount].wiUprOprtngRange 
        || astCommonDatabase->iReadVal < wstProcessThreadConfig[aiPrcsConfigCount].wiLwrOprtngRange)
    {
        errVal = ERRINVALID;
    }
    else
    {
        /* No process*/
    }
    return errVal;
}
/******************************************************************************
* Function name     :   CheckThresholdRange
* Description       :   check sensor value out of threshold range
* Arguments         :   CommonDatabase *astCommonDatabase - hold read value
                        int aiPrcsConfigCount - index of global structure ProcessThreadConfig
* Return type       :   ERROR_TYPE - return errotype, if 0 NO_ERROR
********************************************************************************/
ERROR_TYPE CheckThresholdRange(CommonDatabase *astCommonDatabase, int aiPrcsConfigCount)
{
    ERROR_TYPE errVal = NO_ERR;
    if(astCommonDatabase->iReadVal > wstProcessThreadConfig[aiPrcsConfigCount].wiUpperThreshold 
        || astCommonDatabase->iReadVal < wstProcessThreadConfig[aiPrcsConfigCount].wiLowerThreshold)
    {
        errVal = ERRINVALID;
    }
    else
    {
        /* No process*/
    }
    return errVal;
}
/*********************************************************************************
* Function name     : ProcessDataNode
* Description       : Process the data node for temperature and
*                     pressure value periodically
* Arguments         : CommonDatabase *astCommonDatabase - hold read value
                        int aiPrcsConfigCount - index of global structure ProcessThreadConfig
* Return type       : ERROR_TYPE - return errotype, if 0 NO_ERROR
***********************************************************************************/
ERROR_TYPE ProcessDataNode(CommonDatabase *astCommonDatabase, int aiPrcsConfigCount)
{
    ERROR_TYPE errVal = NO_ERR;
    if(astCommonDatabase == NULL)
    {
        return ERRINVALID;
    }
    else
    {
        /* No process*/
    }

    if(CheckOperatingRange(astCommonDatabase, aiPrcsConfigCount) == NO_ERR)
    {
        if(CheckThresholdRange(astCommonDatabase, aiPrcsConfigCount) == NO_ERR)
        {
            if( g_ubDebugMode != DEF_CLEAR)
            {
                Printinfo(DEBUG, astCommonDatabase, " ");
            }
            else
            {
                /* No process*/
            }

        }
        else
        {
            Printinfo(WARNING, astCommonDatabase, "out of threshold range!");
        }
    }
    else
    {
        errVal = ERRINVALID;
        Printinfo(WARNING, astCommonDatabase, "Sensor read Over bound!");
    }
    return errVal;
}
/************************************************************
* Function name     : wvdProcessingThread
* Description       : thread for processing the temperature and pressure value periodically
* Arguments         : void *arg - thread id
* Return type       : void
**************************************************************/
void* wvdProcessingThread(void *arg)
{
    int thread_id                    = DEF_CLEAR;
    clock_t lCurrent_processtime     = DEF_CLEAR;
    clock_t lCurrent_processtime_sec = DEF_CLEAR;
    ERROR_TYPE  err_t                = NO_ERR;
    int     aiProcessConfgSize       = DEF_CLEAR;
    int aiPrcsConfigCount            = DEF_CLEAR;
    CommonDatabase astCommonDatabase;

    if(arg == NULL)
    {
        PrintMessage(ERROR, "Processing thread arg is NULL");
        return NULL;
    }
    else
    {
        thread_id = *((int*)arg);
    }
    aiProcessConfgSize = sizeof(wstProcessThreadConfig) / sizeof(wstProcessThreadConfig[0]);
    if(aiProcessConfgSize == DEF_CLEAR)
    {
        PrintMessage(ERROR, "Processing thread config size is zero");
        return NULL;
    }
    else
    {
        /* No process*/
    }

    while(1)
    {
    
        for(aiPrcsConfigCount = 0; aiPrcsConfigCount < aiProcessConfgSize; aiPrcsConfigCount++)
        {
            lCurrent_processtime = clock();
            lCurrent_processtime_sec = (lCurrent_processtime) / CLOCKS_PER_SEC;

            if((lCurrent_processtime_sec - wstProcessThreadConfig[aiPrcsConfigCount].wiLastCheckTime)  
                >= wstProcessThreadConfig[aiPrcsConfigCount].wiCheckingTime)
            {
                (void)memset(&astCommonDatabase, DEF_CLEAR, sizeof(CommonDatabase));
                err_t = RetriveDataNode(wstProcessThreadConfig[aiPrcsConfigCount].param_t, &astCommonDatabase);
                if(err_t == NO_ERR)
                {
                    err_t = ProcessDataNode(&astCommonDatabase, aiPrcsConfigCount);
                    if(err_t == NO_ERR)
                    {
                        wstProcessThreadConfig[aiPrcsConfigCount].wiLastCheckTime = lCurrent_processtime_sec;
                    }
                    else
                    {
                        PrintMessage(WARNING,"Error processing sensor data!");
                    }
                }
                else
                {
                    PrintMessage(WARNING,"Error retrieving data from database!");
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
