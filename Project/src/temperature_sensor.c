#include <stdio.h>
#include <stdlib.h>
#include "../include/temperature_sensor.h"
#include "../include/common.h"

int wswReadTemp( void )
{
    int max = UPPER_TEMP_THRESHOLD + CALIB_CONST;
    int min = LOWER_TEMP_THRESHOLD - CALIB_CONST;
    return (rand() % (max - min + 1)) + min;
}

/* end of file*/

