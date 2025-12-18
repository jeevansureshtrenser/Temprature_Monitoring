#include <stdio.h>
#include "../include/common.h"
#include "../include/processing.h"

extern CommonDatabase wstCommonDatabase;
void* wvdProcessingThread(void *arg)
{

    int thread_id = *((int*)arg);
    ProcessThreadConfig wstProcessThreadConfig[PROCESS_CONFIG_SIZE]= {  { PARAM_TEMP,       LOWER_TEMP_THRESHOLD,   UPPER_TEMP_THRESHOLD,   CHECKING_TEMP_INTERVAL,     0},
                                                                        { PARAM_PRESSURE,   LOWER_PRSURE_THRESHOLD, UPPER_PRSURE_THRESHOLD, CHECKING_PRSSR_INTERVAL,    0} };
    clock_t current_processtime     = 0;
    clock_t current_processtime_sec = 0;
    int     aiReadVal               = 0;
    int     iPrcsConfigCount        = 0;
    Parameter_type param_get_type;
    while(1)
    {
        pthread_mutex_lock(&lock);
        aiReadVal = wstCommonDatabase.iReadVal;
        param_get_type = wstCommonDatabase.param_t;
        pthread_mutex_unlock(&lock);

        current_processtime = clock();
        current_processtime_sec = (current_processtime) / CLOCKS_PER_SEC;
        for( iPrcsConfigCount = 0; iPrcsConfigCount < PROCESS_CONFIG_SIZE; iPrcsConfigCount++ )
        {
            if( wstProcessThreadConfig[iPrcsConfigCount].param_t == param_get_type )
            {
                if( ( current_processtime_sec - wstProcessThreadConfig[iPrcsConfigCount].wiLastCheckTime)  
                    >= wstProcessThreadConfig[iPrcsConfigCount].wiCheckingTime )
                {
                    if( aiReadVal < wstProcessThreadConfig[iPrcsConfigCount].wiLowerThreshold
                        || aiReadVal > wstProcessThreadConfig[iPrcsConfigCount].wiUpperThreshold )
                    {
                        switch( param_get_type )
                        {
                            case PARAM_TEMP     : printMessage(WARNING, PARAM_TEMP, aiReadVal);
                                break;
                            case PARAM_PRESSURE : printMessage(WARNING, PARAM_PRESSURE, aiReadVal);
                                break;
                            default             : printMessage(INVALID, 0, 0);
                                break;
                        }

                    }
                    wstProcessThreadConfig[iPrcsConfigCount].wiLastCheckTime = current_processtime_sec;
                    
                }
            }

        }
    }
    printf(" exit thread : %d", thread_id);
}

/* end of file*/
