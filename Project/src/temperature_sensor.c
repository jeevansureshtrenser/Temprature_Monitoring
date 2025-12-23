/************************************************************
* File name     : temperature_sensor.c
* Description   : source file for temperature sensor
* Author        : Jeevan Suresh
* License       : Copyright (c) 2021 Trenser 
                    All Rights Reserved
**************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "../include/temperature_sensor.h"
#include "../include/common.h"

/*******************Function Declaration*********************/
int wswReadTemp(int *);

/************************************************************
* Function name     : wswReadTemp
* Description       : to return random value for temperature value
* Arguments         : void
* Return type       : int
**************************************************************/
int wswReadTemp(int *aiReadval)
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

