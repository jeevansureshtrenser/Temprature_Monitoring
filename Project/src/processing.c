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
#include "../include/common.h"
#include "../include/processing.h"

/***********************Function Declaration******************/
void* wvdProcessingThread(void *arg);
/************************************************************
* Function name     : wvdPollingThread
* Description       : thread for polling the temperature and pressure value periodically
* Arguments         : void *arg - thread id
* Return type       : void
**************************************************************/
void* wvdProcessingThread(void *arg)
{
    int thread_id = *((int*)arg);
    ProcessThreadConfig wstProcessThreadConfig[PROCESS_CONFIG_SIZE] = 
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

    clock_t lCurrent_processtime     = 0;
    clock_t lCurrent_processtime_sec = 0;
    int     aiReadVal                = 0;
    int     aiPrcsConfigCount        = 0;
    PARAMETER_TYPE param_get_type;
    char cMessage[MSG_SIZE];

    while(1)
    {
        pthread_mutex_lock(&g_pthreadlock);
        aiReadVal = wstCommonDatabase.iReadVal;
        param_get_type = wstCommonDatabase.param_t;
        pthread_mutex_unlock(&g_pthreadlock);

        lCurrent_processtime = clock();
        lCurrent_processtime_sec = (lCurrent_processtime) / CLOCKS_PER_SEC;
        
        for(aiPrcsConfigCount = 0; aiPrcsConfigCount < PROCESS_CONFIG_SIZE; aiPrcsConfigCount++)
        {
            if(wstProcessThreadConfig[aiPrcsConfigCount].param_t == param_get_type)
            {
                if((lCurrent_processtime_sec - wstProcessThreadConfig[aiPrcsConfigCount].wiLastCheckTime)  
                    >= wstProcessThreadConfig[aiPrcsConfigCount].wiCheckingTime)
                {
                    if(aiReadVal < wstProcessThreadConfig[aiPrcsConfigCount].wiLwrOprtngRange
                        || aiReadVal > wstProcessThreadConfig[aiPrcsConfigCount].wiUprOprtngRange)
                    {
                        memset(cMessage, DEF_CLEAR, sizeof(cMessage));
                        switch(param_get_type)
                        {
                            case PARAM_TEMP     : snprintf(cMessage, sizeof(cMessage), " %3d Temperature Sensor Overbound Detected! System Failure!", aiReadVal);
                                break;
                            case PARAM_PRESSURE : snprintf(cMessage, sizeof(cMessage), "%3d Pressure Sensor Overbound detected! System Failure!", aiReadVal);
                                break;
                            default             : /* No process*/
                                break;
                        }
                        printMessage(WARNING, (const char*)cMessage);
                    }
                    else
                    {
                        if(aiReadVal < wstProcessThreadConfig[aiPrcsConfigCount].wiLowerThreshold
                        || aiReadVal > wstProcessThreadConfig[aiPrcsConfigCount].wiUpperThreshold)
                        {
                            memset(cMessage, DEF_CLEAR, sizeof(cMessage));
                            switch(param_get_type)
                            {
                                case PARAM_TEMP     : snprintf(cMessage, sizeof(cMessage), "TEMPERATURE | %3d Degree Celsius  | Out of Range ", aiReadVal);
                                    break;
                                case PARAM_PRESSURE : snprintf(cMessage, sizeof(cMessage), "PRESSURE    | %3d PSI             | Out of Range ", aiReadVal);
                                    break;
                                default             : /* No process*/
                                    break;
                            }
                            printMessage(WARNING, (const char*)cMessage);

                        }
                        else
                        {
                            #ifdef DEBUG_INFO
                                memset(cMessage, DEF_CLEAR, sizeof(cMessage));
                                switch(wstCommonDatabase.param_t)
                                {
                                    case PARAM_TEMP     : snprintf(cMessage,sizeof(cMessage), "TEMPERATURE | %3d Degrees Celsius", aiReadVal);
                                        break;
                                    case PARAM_PRESSURE : snprintf(cMessage,sizeof(cMessage), "PRESSURE    | %3d PSI            ", aiReadVal);
                                        break;
                                    default             : /* No process */
                                        break;
                                }
                                printMessage(DEBUG,(const char*)cMessage);
                            #endif
                        }
                        wstProcessThreadConfig[aiPrcsConfigCount].wiLastCheckTime = lCurrent_processtime_sec;

                    }
                    
                }
            }

        }
    }
    printf(" exit thread : %d", thread_id);
}

/* end of file*/
