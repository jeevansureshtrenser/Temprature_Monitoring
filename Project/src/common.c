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
* Function name     : PrintMessage
* Description       : to print the message based on the parameter type
* Arguments         : WarningType warningType_t - to detect type of 
*                        warning, debug or invalid
*                     Parameter_type param_t - to detect the parameter 
*                        temperature or pressure
*                      int iReadVal - read data
* Return type       : void
*************************************************************************/
void PrintMessage(WARNING_TYPE warningType_t, const char *pcInfo)
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
    if(current_time == DEF_CLEAR)
    {
        printf("| %s |  ERROR getting time info | ERROR   | %s |\n", g_cDateBuffer, pcInfo);
        return;
    }
    elapsed = (double)(current_time - g_lStart_reference) / CLOCKS_PER_SEC;
    current_time_sec = (int)elapsed;
    current_milliseconds = (int)((elapsed - current_time_sec) * ONE_MILLS_IN_SEC);

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
        PrintMessage(ERROR, "Current time retrival failed");
        (void)snprintf(g_cDateBuffer, sizeof(g_cDateBuffer), "0000-00-00");
        return;
    }
    else
    {
        /* No process*/
    }
    // Convert to local time structure
    timeinfo = localtime(&rawtime);
    if(timeinfo == NULL)
    {
        PrintMessage(ERROR, "Local Time retrival failed");
        (void)snprintf(g_cDateBuffer, sizeof(g_cDateBuffer), "0000-00-00");
        return;
    }
    else
    {
        /* No process*/
    }

    // Format the date into a string using strftime
    // %Y = Year, %m = Month, %d = Day; format is YYYY-MM-DD
    strftime(g_cDateBuffer, sizeof(g_cDateBuffer), "%Y-%m-%d", timeinfo);
    if(strlen(g_cDateBuffer) == 0)
    {
        PrintMessage(ERROR, "Format the date into a string failed");
        (void)snprintf(g_cDateBuffer, sizeof(g_cDateBuffer), "0000-00-00");
        return;
    }
    else
    {
        /* No process*/
    }
}
/**********************************************************************
* Function name     : PrintErrInfo
* Description       : to print error information
* Arguments         : int aiErrValue - error value
* Return type       : void
*************************************************************************/
void PrintErrInfo(int aiErrValue)
{
     switch(aiErrValue)
    {
        case EAGAIN: PrintMessage(INVALID, "Error: EAGAIN - resource temporarily unavailable!");
            break;
        case EINVAL: PrintMessage(INVALID, "Error: EINVAL - Invalid argument!");
            break;
        case EPERM: PrintMessage(INVALID, "Error: EPERM - Operation Not Permitted!");
            break;
        default : PrintMessage(INVALID, "Error: UNKNOWN - Error unknown");
            break;
    }
}
/**********************************************************************
* Function name     : ExtractArgumentlist
* Description       : to extract command line arguments
* Arguments         : int argc - argument count
*                   : char *argv[] - argument values
* Return type       : ERROR_TYPE - error type
*************************************************************************/
ERROR_TYPE ExtractArgumentlist(int argc, char *argv[])
{
    ERROR_TYPE err_t = NO_ERR;
    for (int index = 1; index < argc; index++)
    {
        if(argv[index] == NULL)
        {
            err_t = ERRINVALID; 
            PrintMessage(ERROR, "Error: argv element is NULL");
            return err_t;
        }
        else
        {
            if(strstr(argv[index],"--DEBUG") != NULL || strstr(argv[index],"-d") != NULL)
            {
                g_ubDebugMode = DEF_SET;
                PrintMessage(INFO, "Debug mode enabled");
                break;
            }
            else if(strstr(argv[index],"--debug") != NULL)
            {
                g_ubDebugMode = DEF_SET;
                PrintMessage(INFO, "Debug mode enabled");
                break;
            }
            else
            {
                err_t = ERRINVALID; 
                PrintMessage(ERROR, "Error: invalid Argument");
                PrintMessage(INFO, "Type ./build/temperature_pressure_monitor");
                PrintMessage(INFO, "Type for debug ./build/temperature_pressure_monitor --DEBUG or -d");
                break;
            }
            /* No Process*/
        }    
    
    }
    return err_t;
}

/*end of file*/