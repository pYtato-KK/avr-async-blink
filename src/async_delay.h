
#ifndef _ASYNC_DELAY_H
#define _ASYNC_DELAY_H

#include <stdbool.h>
#include <stdint.h>
#include <async/async.h>


typedef struct {
    async_state;
    unsigned long long ms;
    uint16_t start, delay;
    bool overflow;
} delay_info;


void delay_rtc_init();
async delay(delay_info *di);


#endif

