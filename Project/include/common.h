#ifndef COMMON_H_
#define COMMON_H_

#include <time.h>
#include <pthread.h>

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
#define DATE_BUF_SIZE               11
#define CALIB_CONST                 20

#define SUCCESS 0

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

extern clock_t start_reference;

extern char date_buffer[DATE_BUF_SIZE];

extern pthread_mutex_t lock;

void printMessage( WarningType warningType_t, Parameter_type param_t, int iReadVal );

#endif /* COMMON_H_ */