/************************************************************
* File name     : polling.c
* Description   : The file contains the polling thread
* Author        : Jeevan Suresh
* License       : License       : Copyright (c) 2021 Trenser 
                    All Rights Reserved
**************************************************************/
#include <stdio.h>
#include <pthread.h>
#include "../include/common.h"
#include "../include/temperature_sensor.h"
#include "../include/pressure_sensor.h"
#include "../include/polling.h"

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
    PollingThreadConfig wstPollingThreadConfig[POLLING_CONFIG_SIZE]= {  { PARAM_TEMP,       wswReadTemp,    POLLING_TEMP_INTERVAL,  0},
                                                                        { PARAM_PRESSURE,   wswReadPrsure,  POLLING_PRSSR_INTERVAL, 0} };
    clock_t current_polltime        = 0;
    clock_t current_polltime_sec    = 0;
    int aiReadVal                   = 0;
    int aiPollConfigCount           = 0;
    while(1)
    {
        current_polltime = clock();
        current_polltime_sec = (current_polltime) / CLOCKS_PER_SEC;
        for(aiPollConfigCount = 0; aiPollConfigCount < PROCESS_CONFIG_SIZE; aiPollConfigCount++)
        {
            if((current_polltime_sec - wstPollingThreadConfig[aiPollConfigCount].wiLastPollingTime)
                >= wstPollingThreadConfig[aiPollConfigCount].wiPollingTime)
            {
                aiReadVal = wstPollingThreadConfig[aiPollConfigCount].iReadfn();
                pthread_mutex_lock(&g_pthreadlock);
                wstCommonDatabase.iReadVal = aiReadVal;
                wstCommonDatabase.param_t = wstPollingThreadConfig[aiPollConfigCount].param_t;
                pthread_mutex_unlock(&g_pthreadlock);
                wstPollingThreadConfig[aiPollConfigCount].wiLastPollingTime = current_polltime_sec;
            }
        }
    }
    printf(" exit thread : %d", thread_id);
}

/*end of file*/
