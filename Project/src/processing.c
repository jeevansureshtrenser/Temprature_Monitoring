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


/************************************************************
* Function name     : wvdPollingThread
* Description       : thread for polling the temperature and pressure value periodically
* Arguments         : void *arg - thread id
* Return type       : void
**************************************************************/
void* wvdProcessingThread(void *arg)
{
    int thread_id = *((int*)arg);

    clock_t lCurrent_processtime     = DEF_CLEAR;
    clock_t lCurrent_processtime_sec = DEF_CLEAR;
    int     aiReadVal                = DEF_CLEAR;
    int     aiPrcsConfigCount        = DEF_CLEAR;

    int aiProcessConfgSize = sizeof(wstProcessThreadConfig) / sizeof(wstProcessThreadConfig[0]);
    CommonDatabase astCommonDatabase;
    char cMessage[MSG_SIZE];

    while(1)
    {
        lCurrent_processtime = clock();
        lCurrent_processtime_sec = (lCurrent_processtime) / CLOCKS_PER_SEC;
        
        for(aiPrcsConfigCount = 0; aiPrcsConfigCount < aiProcessConfgSize; aiPrcsConfigCount++)
        {

            if((lCurrent_processtime_sec - wstProcessThreadConfig[aiPrcsConfigCount].wiLastCheckTime)  
                >= wstProcessThreadConfig[aiPrcsConfigCount].wiCheckingTime)
            {
                pthread_mutex_lock(&g_pthreadlock);
                aiReadVal = wstRetrive_data_node(wstProcessThreadConfig[aiPrcsConfigCount].param_t, &astCommonDatabase);
                pthread_mutex_unlock(&g_pthreadlock);
                if(aiReadVal == SUCCESS)
                {
                    memset(cMessage, DEF_CLEAR, sizeof(cMessage));
                    switch(astCommonDatabase.param_t)
                    {
                        case PARAM_TEMP     : snprintf(cMessage,sizeof(cMessage), "TEMPERATURE | %3d Degrees Celsius", astCommonDatabase.iReadVal);
                            break;
                        case PARAM_PRESSURE : snprintf(cMessage,sizeof(cMessage), "PRESSURE    | %3d PSI            ", astCommonDatabase.iReadVal);
                            break;
                        default             : /* No process */
                            break;
                    }
                    printMessage(DEBUG, cMessage);
                    wstProcessThreadConfig[aiPrcsConfigCount].wiLastCheckTime = lCurrent_processtime_sec;
                
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
