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
int wswReadTemp( void );

/************************************************************
* Function name     : wswReadTemp
* Description       : to return random value for temperature value
* Arguments         : void
* Return type       : int
**************************************************************/
int wswReadTemp( void )
{
    int max = UPPER_TEMP_THRESHOLD + CALIB_CONST;
    int min = LOWER_TEMP_THRESHOLD - CALIB_CONST;
    return (rand() % (max - min + 1)) + min;
}

/* end of file*/

