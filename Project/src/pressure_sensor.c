#include <stdio.h>
#include <stdlib.h>
#include "../include/pressure_sensor.h"
#include "../include/common.h"

int wswReadPrsure( void )
{
    int max = UPPER_PRSURE_THRESHOLD + CALIB_CONST;
    int min = LOWER_PRSURE_THRESHOLD - CALIB_CONST;
    return (rand() % (max - min + 1)) + min;
}

/* end of file*/

