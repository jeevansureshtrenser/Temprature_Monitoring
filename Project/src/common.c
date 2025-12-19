/************************************************************
* File name     : common.c
* Description   : common file for project
* Author        : Jeevan Suresh
* License       : Copyright (c) 2021 Trenser 
                    All Rights Reserved
**************************************************************/
/**********************Include Files**************************/
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "../include/common.h"

/*********************Global Variable Declaration*************/

CommonDatabase wstCommonDatabase;

pthread_mutex_t g_pthreadlock = PTHREAD_MUTEX_INITIALIZER;

clock_t g_lStart_reference = 0;

char g_cDateBuffer[DATE_BUF_SIZE];

/**********************************************************************
* Function name     : printMessage
* Description       : to print the message based on the parameter type
* Arguments         : WarningType warningType_t - to detect type of 
*                        warning, debug or invalid
*                     Parameter_type param_t - to detect the parameter 
*                        temperature or pressure
*                      int iReadVal - read data
* Return type       : void
*************************************************************************/
void printMessage(WARNING_TYPE warningType_t, const char *pcInfo)
{
    clock_t current_time;
    double elapsed;
    clock_t current_time_sec;
    clock_t current_milliseconds;

    current_time = clock();
    elapsed = (double)(current_time - g_lStart_reference) / CLOCKS_PER_SEC;
    current_time_sec = (int)elapsed;
    current_milliseconds = (int)((elapsed - current_time_sec) * 1000);

    switch(warningType_t)
    {
        case WARNING: 
                        printf("| %s | %6ld.%06ld | WARNING | %s |\n", g_cDateBuffer, current_time_sec, current_milliseconds, pcInfo);
            break;
        case DEBUG:

                        printf("| %s | %6ld.%06ld |  DEBUG  | %s |\n", g_cDateBuffer, current_time_sec, current_milliseconds, pcInfo);
            break;
        case INVALID:
                        printf("| %s | %6ld.%06ld | INVALID | %s |\n", g_cDateBuffer, current_time_sec, current_milliseconds, pcInfo);
            break;
        case INFO:
                        printf("| %s | %6ld.%06ld | INFO    | %s |\n", g_cDateBuffer, current_time_sec, current_milliseconds, pcInfo);
            break;
        default:
            break;
    }

}

/*end of file*/