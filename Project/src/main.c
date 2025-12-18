#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../include/common.h"
#include "../include/polling.h"
#include "../include/processing.h"

extern void* wvdProcessingThread(void *arg);
extern void* wvdPollingThread(void *arg);


int main( void )
{
    // Declare handle for threads
    pthread_t pollingthread, process_thread;

    int iPollThreadID    = 1;
    int iProcessThreadID = 2;

    //Get the thread start time
    start_reference =  clock();
    time_t rawtime;
    struct tm *timeinfo = NULL;

    // Get the current time (seconds since the Unix epoch)
    time(&rawtime);

    // Convert to local time structure
    timeinfo = localtime(&rawtime);

    // Format the date into a string using strftime
    // %Y = Year, %m = Month, %d = Day; format is YYYY-MM-DD
    strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d", timeinfo);

    // Create thread for Polling
    if (pthread_create(&pollingthread, NULL, wvdPollingThread, &iPollThreadID) == 0) 
    { 
        // Create thread for processing
        if (pthread_create(&process_thread, NULL, wvdProcessingThread, &iProcessThreadID) == 0)
        {
            // Wait for both threads to finish 
            pthread_join(pollingthread, NULL);
            pthread_join(process_thread, NULL);
        }
        else
        { 
            printf("process thread");
        }
       
    }
    else
    {
        printf(" pollingthread error");
    }

    printf("Both threads finished execution.\n");
    return 0;
}


