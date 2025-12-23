/************************************************************
* File name     : polling.c
* Description   : The file contains the polling thread
* Author        : Jeevan Suresh
* License       : License       : Copyright (c) 2021 Trenser 
                    All Rights Reserved
**************************************************************/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "../include/common.h"
#include "../include/temperature_sensor.h"
#include "../include/pressure_sensor.h"
#include "../include/polling.h"
#include "../include/database.h"


static PollingThreadConfig wstPollingThreadConfig[] = 
{  
    {
        PARAM_TEMP,
        wswReadTemp,
        POLLING_TEMP_INTERVAL,
        0
    },
    {
        PARAM_PRESSURE,
        wswReadPrsure,
        POLLING_PRSSR_INTERVAL,
        0
    }
};
/******************************Function Declaration******************************/
void* wvdPollingThread(void *arg);

/************************************************************
* Function name     : wvdPollingThread
* Description       : thread for polling the temperature and pressure value periodically
* Arguments         : void *arg - thread id
* Return type       : void
**************************************************************/
void* wvdPollingThread(void *arg)
{
    int thread_id = *((int*)arg);

    clock_t current_polltime        = DEF_CLEAR;
    clock_t current_polltime_sec    = DEF_CLEAR;
    int aiReadVal                   = DEF_CLEAR;
    int aiReadSts                   = DEF_CLEAR;
    int aiPollConfigCount           = DEF_CLEAR;
    int aiPollConfgSize             = DEF_CLEAR;
    CommonDatabase astCommonDatabase;
    aiPollConfgSize = sizeof(wstPollingThreadConfig) / sizeof(wstPollingThreadConfig[0]);
    while(1)
    {
        for(aiPollConfigCount = 0; aiPollConfigCount < aiPollConfgSize; aiPollConfigCount++)
        {
            current_polltime = clock();
            current_polltime_sec = (current_polltime) / CLOCKS_PER_SEC;
            if((current_polltime_sec - wstPollingThreadConfig[aiPollConfigCount].wiLastPollingTime)
                >= wstPollingThreadConfig[aiPollConfigCount].wiPollingTime)
            {
                aiReadSts = wstPollingThreadConfig[aiPollConfigCount].iReadfn(&aiReadVal);
                if(aiReadSts == SUCCESS)
                {
                    astCommonDatabase.iReadVal = aiReadVal;
                    astCommonDatabase.param_t = wstPollingThreadConfig[aiPollConfigCount].param_t;
                    pthread_mutex_lock(&g_pthreadlock);
                    aiReadSts = ucCheck_and_update_node(&astCommonDatabase);
                    pthread_mutex_unlock(&g_pthreadlock);
                    if(aiReadSts == NO_ERR)
                    {
                        wstPollingThreadConfig[aiPollConfigCount].wiLastPollingTime = current_polltime_sec;
                    }
                }
                else
                {
                    printMessage(INVALID, "Sensor read Error!");
                }

            }
            else
            {
                /* No process*/
            }
        }
    }
    printf(" exit thread : %d", thread_id);
}

/*end of file*/
