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
#include <string.h>
#include <errno.h>
#include "../include/common.h"

/*********************Global Variable Declaration*************/
pthread_mutex_t g_pthreadlock   = PTHREAD_MUTEX_INITIALIZER;
clock_t g_lStart_reference      = DEF_CLEAR;
unsigned char g_ubDebugMode     = DEF_CLEAR;
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
    clock_t current_time            = DEF_CLEAR;
    double elapsed                  = DEF_CLEAR;
    clock_t current_time_sec        = DEF_CLEAR;
    clock_t current_milliseconds    = DEF_CLEAR;
    if(pcInfo == NULL)
    {
        return;
    }
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
        case ERROR:
                        printf("| %s | %6ld.%06ld | ERROR   | %s |\n", g_cDateBuffer, current_time_sec, current_milliseconds, pcInfo);
            break;
        case INFO:
                        printf("| %s | %6ld.%06ld | INFO    | %s |\n", g_cDateBuffer, current_time_sec, current_milliseconds, pcInfo);
            break;
        default:
            break;
    }

}
/**********************************************************************
* Function name     : InitDateBuffer
* Description       : to Initialize global date buffer
* Arguments         : void
* Return type       : void
*************************************************************************/
void InitDateBuffer(void)
{
    memset(g_cDateBuffer, DEF_CLEAR, sizeof(g_cDateBuffer));
    //Get the thread start time
    g_lStart_reference =  clock();

    // declare the time structure
    time_t rawtime = DEF_CLEAR;
    struct tm *timeinfo = NULL;

    // Get the current time (seconds since the Unix epoch)
    rawtime = time(NULL);
    if(rawtime == DEF_CLEAR )
    {
        printMessage(INFO, "Time retrival failed");
    }
    else
    {
        /* No process*/
    }
    // Convert to local time structure
    timeinfo = localtime(&rawtime);
    if(timeinfo == NULL)
    {
        printMessage(INFO, "Time retrival failed");
    }
    else
    {
        /* No process*/
    }

    // Format the date into a string using strftime
    // %Y = Year, %m = Month, %d = Day; format is YYYY-MM-DD
    strftime(g_cDateBuffer, sizeof(g_cDateBuffer), "%Y-%m-%d", timeinfo);
}
/**********************************************************************
* Function name     : printErrInfo
* Description       : to print error information
* Arguments         : int aiErrValue - error value
* Return type       : void
*************************************************************************/
void printErrInfo(int aiErrValue)
{
     switch(aiErrValue)
    {
        case EAGAIN: printMessage(INVALID, "Error: EAGAIN - resource temporarily unavailable!");
            break;
        case EINVAL: printMessage(INVALID, "Error: EINVAL - Invalid argument!");
            break;
        case EPERM: printMessage(INVALID, "Error: EPERM - Operation Not Permitted!");
            break;
        default : printMessage(INVALID, "Error: UNKNOWN - Error unknown");
            break;
    }
}

ERROR_TYPE ExtractArgumentlist(int argc, char *argv[])
{
    ERROR_TYPE err_t = NO_ERR;
    if (argv == NULL)
    { 
        printMessage(ERROR,"Error: argv is NULL\n");
        return ERRINVALID; 
    }
    else
    {
        /* No Process*/
    }
    for (int index = 1; index < argc; index++)
    { 
        if(strstr(argv[index],"--DEBUG") != NULL)
        {
            g_ubDebugMode = 1;
            break;
        }
        else if(strstr(argv[index],"-d") != NULL)
        {
            g_ubDebugMode = 1;
            break;
        }
        else
        {
            err_t = ERRINVALID; 
            printMessage(ERROR, "Error: invalid Argument");
            printMessage(INFO, "Type ./build/temperature_pressure_monitor");
            printMessage(INFO, "Type for debug ./build/temperature_pressure_monitor --DEBUG or -d");
            
        }
    
    }
    return err_t;
}

/*end of file*/