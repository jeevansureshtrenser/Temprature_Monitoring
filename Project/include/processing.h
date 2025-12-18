#ifndef __PROCESSING_H__
#define __PROCESSING_H__

typedef struct
{
    Parameter_type param_t;
    int wiLowerThreshold;
    int wiUpperThreshold;
    long int wiCheckingTime;
    long int wiLastCheckTime;
}ProcessThreadConfig;

void* wvdProcessingThread(void *arg);

#endif /*__PROCESSING_H__*/
