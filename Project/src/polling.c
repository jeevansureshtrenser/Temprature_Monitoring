#include <stdio.h>
#include <pthread.h>
#include "../include/common.h"
#include "../include/polling.h"

extern CommonDatabase wstCommonDatabase;

extern int wswReadTemp( void );
extern int wswReadPrsure( void );

void* wvdPollingThread(void *arg)
{
    int thread_id = *((int*)arg);
    PollingThreadConfig wstPollingThreadConfig[POLLING_CONFIG_SIZE]= {  { PARAM_TEMP,       wswReadTemp,    POLLING_TEMP_INTERVAL,  0},
                                                                        { PARAM_PRESSURE,   wswReadPrsure,  POLLING_PRSSR_INTERVAL, 0} };
    clock_t current_polltime        = 0;
    clock_t current_polltime_sec    = 0;
    int aiReadVal                   = 0;
    int     iPollConfigCount        = 0;
    while(1)
    {
        current_polltime = clock();
        current_polltime_sec = (current_polltime) / CLOCKS_PER_SEC;
        for( iPollConfigCount = 0; iPollConfigCount < PROCESS_CONFIG_SIZE; iPollConfigCount++)
        {
            if( ( current_polltime_sec - wstPollingThreadConfig[iPollConfigCount].wiLastPollingTime )  >= wstPollingThreadConfig[iPollConfigCount].wiPollingTime )
            {
                aiReadVal = wstPollingThreadConfig[iPollConfigCount].iReadfn();
                pthread_mutex_lock(&lock);
                wstCommonDatabase.iReadVal = aiReadVal;
                wstCommonDatabase.param_t = wstPollingThreadConfig[iPollConfigCount].param_t;
                pthread_mutex_unlock(&lock);
                #ifdef DEBUG_INFO
                    printMessage(DEBUG, wstCommonDatabase.param_t, wstCommonDatabase.iReadVal);
                #endif
                wstPollingThreadConfig[iPollConfigCount].wiLastPollingTime = current_polltime_sec;
            }
        }
    }
    printf(" exit thread : %d", thread_id);
}

/*end of file*/
