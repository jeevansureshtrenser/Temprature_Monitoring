/************************************************************
* File name     : processing.c
* Description   : source file for pressure sensor
* Author        : Jeevan Suresh
* License       : 
**************************************************************/
/******************************Include Files************************************/
#include <stdio.h>
#include <stdlib.h>
#include "../include/pressure_sensor.h"

/******************************Function Declaration******************************/
ERROR_TYPE wswReadPrsure(int *);

/************************************************************
* Function name     : wswReadPrsure
* Description       : to return random value for pressure value
* Arguments         : void
* Return type       : int
**************************************************************/
ERROR_TYPE wswReadPrsure(int *aiReadval)
{
    int max = UPPER_PRSURE_THRESHOLD + CALIB_CONST;
    int min = LOWER_PRSURE_THRESHOLD - CALIB_CONST;
    *aiReadval = (rand() % (max - min + 1)) + min;
    if(aiReadval == DEF_CLEAR)
    {
        return ERRINVALID;
    }
    else
    {
        /* No Process*/
    }
    return NO_ERR;
}

/* end of file*/

