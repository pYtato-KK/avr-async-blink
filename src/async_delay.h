
#ifndef _ASYNC_DELAY_H
#define _ASYNC_DELAY_H

#include <stdbool.h>
#include <stdint.h>
#include <async/async.h>

typedef struct {
    async_state;
    uint16_t start, delay;
    bool overflow;
} delay_info;


typedef struct {
    async_state;
    unsigned long long ms;
    delay_info *ret;
} newdelay_state;


void delay_rtc_init();
async newdelay(newdelay_state*);
async delay(delay_info *di);


#endif

