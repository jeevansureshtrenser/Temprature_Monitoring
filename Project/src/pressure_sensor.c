/************************************************************
* File name     : processing.c
* Description   : source file for pressure sensor
* Author        : Jeevan Suresh
* License       : 
**************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "../include/pressure_sensor.h"
#include "../include/common.h"

/******************************Function Declaration******************************/
int wswReadPrsure(int *);

/************************************************************
* Function name     : wswReadPrsure
* Description       : to return random value for pressure value
* Arguments         : void
* Return type       : int
**************************************************************/
int wswReadPrsure(int *aiReadval)
{
    int max = UPPER_PRSURE_THRESHOLD + CALIB_CONST;
    int min = LOWER_PRSURE_THRESHOLD - CALIB_CONST;
    *aiReadval = (rand() % (max - min + 1)) + min;
    if(aiReadval == DEF_CLEAR)
    {
        return ERRINVALID;
    }
    return NO_ERR;
}

/* end of file*/

