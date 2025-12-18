#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "../include/common.h"

CommonDatabase wstCommonDatabase;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
clock_t start_reference = 0;
char date_buffer[DATE_BUF_SIZE];

void printMessage( WarningType warningType_t, Parameter_type param_t, int iReadVal )
{
    clock_t current_time;
    double elapsed;
    clock_t current_time_sec;
    clock_t current_milliseconds;
    current_time = clock();
    elapsed = (double)(current_time /*- start_reference*/) / CLOCKS_PER_SEC;
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

/*end of file*/