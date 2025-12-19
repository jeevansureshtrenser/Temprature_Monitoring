/************************************************************
* File name     : main.c
* Description   : This file is the entry point to the system
* Author        : Jeevan Suresh
* License       : 
**************************************************************/

/******************************Include Files******************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include "../include/common.h"
#include "../include/polling.h"
#include "../include/processing.h"

/************************************************************
* File name     : main
* Description   : entry point of the application
* argument      : void
* Return        : int
**************************************************************/
int main( void )
{
    // Declare handle for threads
    pthread_t pollingthread, process_thread;

    int iPollThreadID       = POLLING_THREAD_ID;
    int iProcessThreadID    = PROCESS_THREAD_ID;
    int aiThreadReturn      = DEF_CLEAR;

    //Get the thread start time
    g_lStart_reference =  clock();

    // declare the time structure
    time_t rawtime;
    struct tm *timeinfo = NULL;

    // Get the current time (seconds since the Unix epoch)
    time(&rawtime);

    // Convert to local time structure
    timeinfo = localtime(&rawtime);

    // Format the date into a string using strftime
    // %Y = Year, %m = Month, %d = Day; format is YYYY-MM-DD
    strftime(g_cDateBuffer, sizeof(g_cDateBuffer), "%Y-%m-%d", timeinfo);

    // Create thread for Polling
    if ((aiThreadReturn = pthread_create(&pollingthread, NULL, wvdPollingThread, &iPollThreadID)) == 0) 
    {
        printMessage(INFO, "Polling Thread created successfully");
        // Create thread for processing
        if (pthread_create(&process_thread, NULL, wvdProcessingThread, &iProcessThreadID) == 0)
        {
            printMessage(INFO, "Processing Thread created successfully");
            // Wait for both threads to finish 
            pthread_join(pollingthread, NULL);
            pthread_join(process_thread, NULL);
        }
        else
        {
            switch(aiThreadReturn)
            {
                case EAGAIN: printMessage(INVALID, "EAGAIN : Processing Thread creation failed");
                    break;
                case EINVAL: printMessage(INVALID, "EINVAL : Processing Thread creation failed");
                    break;
                case EPERM: printMessage(INVALID, "EPERM : Processing Thread creation failed");
                    break;
                default : printMessage(INVALID, "UNKNOWN : Processing Thread creation failed");
            }
            
        }
       
    }
    else
    {
        switch(aiThreadReturn)
        {
            case EAGAIN: printMessage(INVALID, "EAGAIN : Polling Thread creation failed");
                break;
            case EINVAL: printMessage(INVALID, "EINVAL : Polling Thread creation failed");
                break;
            case EPERM: printMessage(INVALID, "EPERM : Polling Thread creation failed");
                break;
            default : printMessage(INVALID, "UNKNOWN : Polling Thread creation failed");
                break;
        }
    }

    printMessage(INFO, "Thread creation failed! System Shut down!");
    return 0;
}


/* end of file*/

