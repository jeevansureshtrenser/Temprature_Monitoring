#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define UPPER_TEMP_THRESHOLD        70  //Upper threshold value for temp in degrees Celsius
#define LOWER_TEMP_THRESHOLD        -10 //Lower threshold value for temp in degrees Celsius
#define UPPER_PRSURE_THRESHOLD      100 //Upper threshold value for Pressure in PSI
#define LOWER_PRSURE_THRESHOLD      0   //Lower threshold value for pressure in PSI
#define POLLING_TEMP_INTERVAL       2   //Temperature polling interval
#define POLLING_PRSSR_INTERVAL      5   //Pressure Polling Interval
#define CHECKING_TEMP_INTERVAL      5   //Temperature Check interval
#define CHECKING_PRSSR_INTERVAL     10   //Pressure Check interval

#define POLLING_CONFIG_SIZE         2
#define PROCESS_CONFIG_SIZE         2
#define CALIB_CONST                 20

#define SUCCESS 0

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

typedef enum
{
    PARAM_TEMP,
    PARAM_PRESSURE,
    PARAM_CONFIG
}Parameter_type;

typedef struct
{
    Parameter_type param_t;
    int iReadVal;
}CommonDatabase;

typedef struct
{
    Parameter_type param_t;
    int (*iReadfn)(void);
    long int wiPollingTime;
    long int wiLastPollingTime;
}PollingThreadConfig;

typedef struct
{
    Parameter_type param_t;
    int wiLowerThreshold;
    int wiUpperThreshold;
    long int wiCheckingTime;
    long int wiLastCheckTime;
}ProcessThreadConfig;

CommonDatabase wstCommonDatabase;

int wswReadTemp( void )
{
    int max = UPPER_TEMP_THRESHOLD + CALIB_CONST;
    int min = LOWER_TEMP_THRESHOLD - CALIB_CONST;
    return (rand() % (max - min + 1)) + min;
}

int wswReadPrsure( void )
{
    int max = UPPER_PRSURE_THRESHOLD + CALIB_CONST;
    int min = LOWER_PRSURE_THRESHOLD - CALIB_CONST;
    return (rand() % (max - min + 1)) + min;
}

void* wvdPollingThread(void *arg)
{
    int thread_id = *((int*)arg);
    PollingThreadConfig wstPollingThreadConfig[POLLING_CONFIG_SIZE]= { { PARAM_TEMP, wswReadTemp, POLLING_TEMP_INTERVAL, 0},
                                                                        { PARAM_PRESSURE, wswReadPrsure, POLLING_PRSSR_INTERVAL, 0} };
    clock_t current_polltime        = 0;
    clock_t current_polltime_sec    = 0;
    int aiReadVal                   = 0;
    int     iPollConfigCount        = 0;
    clock_t start =  clock();
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
                printf(" data %d, param %d \n", wstCommonDatabase.iReadVal, wstCommonDatabase.param_t );
                wstPollingThreadConfig[iPollConfigCount].wiLastPollingTime = current_polltime_sec;
            }
        }
    }
}

void* wvdProcessingThread(void *arg)
{

    int thread_id = *((int*)arg);
    ProcessThreadConfig wstProcessThreadConfig[PROCESS_CONFIG_SIZE]= {  { PARAM_TEMP, LOWER_TEMP_THRESHOLD, UPPER_TEMP_THRESHOLD, CHECKING_TEMP_INTERVAL, 0},
                                                                        { PARAM_PRESSURE, LOWER_PRSURE_THRESHOLD, UPPER_PRSURE_THRESHOLD, CHECKING_PRSSR_INTERVAL, 0} };
    clock_t current_processtime     = 0;
    clock_t current_processtime_sec = 0;
    int     aiReadVal               = 0;
    int     iPrcsConfigCount        = 0;
    Parameter_type param_get_type;
    clock_t start =  clock();
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
                            case PARAM_TEMP     : printf(" Read  Temp: %d   : out of range\n", aiReadVal);
                                break;
                            case PARAM_PRESSURE : printf(" Read  Pressure: %d   : out of range\n", aiReadVal);
                                break;
                            default             : printf(" Invalid Parameter \n");
                                break;
                        }

                    }
                    wstProcessThreadConfig[iPrcsConfigCount].wiLastCheckTime = current_processtime_sec;
                    
                }
            }

        }
    }
}

int main( void )
{
    // Declare handle for threads
    pthread_t pollingthread, process_thread;

    int iPollThreadID    = 1;
    int iProcessThreadID = 2;

    // Create thread for Polling
    if (pthread_create(&pollingthread, NULL, wvdPollingThread, &iPollThreadID) != SUCCESS) 
    { 
        printf(" pollingthread error");
    }

    // Create thread for processing
    if (pthread_create(&process_thread, NULL, wvdProcessingThread, &iProcessThreadID) != SUCCESS)
    { 
        printf("process thread");
    }

    // Wait for both threads to finish 
    pthread_join(pollingthread, NULL);
    pthread_join(process_thread, NULL);
    printf("Both threads finished execution.\n");
    return 0;
}