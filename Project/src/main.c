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
int main(int argc, char *argv[])
{
    // Declare handle for threads
    pthread_t pollingthread     = DEF_CLEAR;
    pthread_t process_thread    = DEF_CLEAR;

    int iPollThreadID       = POLLING_THREAD_ID;
    int iProcessThreadID    = PROCESS_THREAD_ID;
    int aiThreadReturn      = DEF_CLEAR;

    // Create date buffer, store current date
    InitDateBuffer();

    if(ExtractArgumentlist(argc, argv) != NO_ERR)
    {
        printMessage(INFO, "System Shut down!");
        return 0;
    }

    // Create thread for Polling
    if ((aiThreadReturn = pthread_create(&pollingthread, NULL, wvdPollingThread, &iPollThreadID)) == SUCCESS) 
    {
        printMessage(INFO, "Polling Thread created successfully");
        // Create thread for processing
        if ((aiThreadReturn = pthread_create(&process_thread, NULL, wvdProcessingThread, &iProcessThreadID)) == SUCCESS)
        {
            printMessage(INFO, "Processing Thread created successfully");
            // Wait for both threads to finish 
            pthread_join(pollingthread, NULL);
            pthread_join(process_thread, NULL);
        }
        else
        {
            printMessage(INVALID, "Processing Thread creation failed");
            printErrInfo(aiThreadReturn);
        }
       
    }
    else
    {
            printMessage(INVALID, "Polling Thread creation failed");
            printErrInfo(aiThreadReturn);
    }

    printMessage(INFO, "Thread creation failed! System Shut down!");
    return 0;
}


/* end of file*/

