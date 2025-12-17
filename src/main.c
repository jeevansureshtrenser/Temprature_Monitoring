#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define UPPER_TEMP_THRESHOLD        70  //Upper threshold value for temp in degrees Celsius
#define LOWER_TEMP_THRESHOLD        -10 //Lower threshold value for temp in degrees Celsius
#define UPPER_PRSURE_THRESHOLD      100 //Upper threshold value for Pressure in PSI
#define LOWER_PRSURE_THRESHOLD      0   //Lower threshold value for pressure in PSI
#define POLLING_TEMP_INTERVAL       2   //Temperature polling interval
#define POLLING_PRSSR_INTERVAL      4   //Pressure Polling Interval
#define CHECKING_TEMP_INTERVAL      5   //Temperature Check interval
#define CHECKING_PRSSR_INTERVAL     6   //Pressure Check interval

#define POLLING_CONFIG_SIZE         2
#define PROCESS_CONFIG_SIZE         2
#define CALIB_CONST                 20

#define SUCCESS 0


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
clock_t start;
char date_buffer[11];

typedef enum
{
    WARNING,
    DEBUG,
    INVALID
}WarningType;

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

void printMessage( WarningType warningType_t, Parameter_type param_t, int iReadVal )
{
    clock_t current_time;
    double elapsed;
    clock_t current_time_sec;
    clock_t current_milliseconds;
    current_time = clock();
    elapsed = (double)(current_time - start) / CLOCKS_PER_SEC;
    current_time_sec = (int)elapsed;
    current_milliseconds = (int)((elapsed - current_time_sec) * 1000);
    switch( warningType_t )
    {
        case WARNING: 
                        if( param_t == PARAM_TEMP )
                        {
                            printf("| %s | %6ld.%06ld | WARNING | TEMPERATURE | %3d degrees Celsius    | Out of range |\n", date_buffer, current_time_sec, current_milliseconds, iReadVal);
                        }
                        else if( param_t == PARAM_PRESSURE )
                        {
                            printf("| %s | %6ld.%06ld | WARNING | PRESSURE    | %3d PSI                | Out of range |\n", date_buffer, current_time_sec, current_milliseconds, iReadVal);
                        }
            break;
        case DEBUG:
                        if( param_t == PARAM_TEMP )
                        {
                            printf("| %s | %6ld.%06ld |  DEBUG  | TEMPERATURE | %3d degrees Celsius    |\n", date_buffer, current_time_sec, current_milliseconds, iReadVal);
                        }
                        else if( param_t == PARAM_PRESSURE )
                        {
                            printf("| %s | %6ld.%06ld |  DEBUG  | PRESSURE    | %3d PSI                |\n", date_buffer, current_time_sec, current_milliseconds, iReadVal);
                        }
            break;
        case INVALID:
                        printf("| %s | %6ld.%06ld |  DEBUG  | Invalid Operations |\n", date_buffer, current_time_sec, current_milliseconds );
            break;
        default:
            break;
    }
}

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
}

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
}

int main( void )
{
    // Declare handle for threads
    pthread_t pollingthread, process_thread;

    int iPollThreadID    = 1;
    int iProcessThreadID = 2;

    //Get the thread start time
    start =  clock();
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
    if (pthread_create(&pollingthread, NULL, wvdPollingThread, &iPollThreadID) == SUCCESS) 
    { 
        // Create thread for processing
        if (pthread_create(&process_thread, NULL, wvdProcessingThread, &iProcessThreadID) == SUCCESS)
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