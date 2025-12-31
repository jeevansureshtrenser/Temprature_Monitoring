/************************************************************
* File name     : common.h
* Description   : header file for common
* Author        : Jeevan Suresh
* License       : 
**************************************************************/
#ifndef COMMON_H_
#define COMMON_H_

//******************************* Include Files ******************************* 
#include <time.h>
#include <pthread.h>

/******************************Macro Definitions******************************/

#define UPPER_TEMP_THRESHOLD            70  // Upper threshold value for temp in degrees Celsius
#define LOWER_TEMP_THRESHOLD            -10 // Lower threshold value for temp in degrees Celsius
#define UPPER_PRSURE_THRESHOLD          100 // Upper threshold value for Pressure in PSI
#define LOWER_PRSURE_THRESHOLD          0   // Lower threshold value for pressure in PSI
#define TEMP_LOWER_OPERATING_RANGE      -20 // Lower operating value for temp in degrees Celsius
#define TEMP_UPPER_OPERATING_RANGE      150 // Upper operating value for temp in degrees Celsius
#define PRESSURE_LOWER_OPERATING_RANGE  -10 // Lower Operating value for pressure in PSI
#define PRESSURE_UPPER_OPERATING_RANGE  200 // Upper Operating value for Pressure in PSI
#define POLLING_TEMP_INTERVAL           2   // Temperature polling interval
#define POLLING_PRSSR_INTERVAL          4   // Pressure Polling Interval
#define CHECKING_TEMP_INTERVAL          5   // Temperature Check interval
#define CHECKING_PRSSR_INTERVAL         6   // Pressure Check interval

#define POLLING_CONFIG_SIZE             2   // Size of the polling config structure
#define PROCESS_CONFIG_SIZE             2   // Size of the process config structure
#define DATE_BUF_SIZE                   11  // Size of the date buffer size
#define CALIB_CONST                     20  // constant for exceed the threshold

#define SUCCESS                         0   // For success return
#define DEF_CLEAR                       0
#define DEF_SET                         1
#define MSG_SIZE                        64

#define ONE_MILLS_IN_SEC                1000

/******************************Global Variable declaration******************************/
typedef enum
{
    WARNING,
    DEBUG,
    INFO,
    ERROR,
    INVALID
}WARNING_TYPE;

typedef enum
{
    NO_ERR,
    ERRCREATN,
    ERRINVALID,
    ERR_NOT_FOUND
}ERROR_TYPE;

typedef enum
{
    PARAM_TEMP,
    PARAM_PRESSURE,
    PARAM_CONFIG
}PARAMETER_TYPE;

typedef struct
{
    PARAMETER_TYPE param_t;
    int iReadVal;
}CommonDatabase;

extern clock_t g_lStart_reference;
extern unsigned char g_ubDebugMode;

extern char g_cDateBuffer[DATE_BUF_SIZE];

extern pthread_mutex_t g_pthreadlock;

//extern CommonDatabase wstCommonDatabase;

/******************************FUnction declaration******************************/
void printMessage(WARNING_TYPE warningType_t, const char *pcInfo);
void InitDateBuffer(void);
void printErrInfo(int aiErrValue);
ERROR_TYPE ExtractArgumentlist(int argc, char *argv[]);
#endif /* COMMON_H_ */