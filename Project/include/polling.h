#ifndef __POLLING_H__
#define __POLLING_H_

typedef struct
{
    Parameter_type param_t;
    int (*iReadfn)(void);
    long int wiPollingTime;
    long int wiLastPollingTime;

}PollingThreadConfig;

void* wvdPollingThread(void *arg);

#endif /*__POLLING_H_*/
