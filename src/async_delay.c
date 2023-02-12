
#include <avr/io.h>
#include "async_delay.h"
#define F_CPU 4e6

static const unsigned long long F_RTC = 2048;
static unsigned int rc = 0;


void delay_rtc_init() {
    RTC.CLKSEL = RTC_CLKSEL_OSC32K_gc;

    while (RTC.STATUS & RTC_CTRLABUSY_bm);
    RTC.CTRLA = RTC_RUNSTDBY_bm | RTC_PRESCALER_DIV16_gc | RTC_RTCEN_bm;
}


async newdelay(newdelay_state *pt) {
    async_begin(pt);

    await_while(RTC.STATUS & RTC_CNTBUSY_bm);

    if (rc == 0) {
        RTC.CNT = 0;
    }

    uint16_t start = RTC.CNT;
    uint16_t delay = ((pt->ms * F_RTC) / 1000);


    bool overflow = false;

    if ((start + delay) < start) {
        overflow = true;
    }
    
    rc++;

    delay_info di = {
        0,
        start, 
        delay, 
        overflow
    };

    async_init(&di);

    *(pt->ret) = di;

    async_end;
}


inline async delay(delay_info *di) {
    async_begin(di);

    while (1) {
        async_yield;
        await_while(RTC.STATUS & RTC_CNTBUSY_bm);
        uint16_t cnt = RTC.CNT;
    
        if (di->overflow) {
            if (cnt >= di->start) {
                continue;
            }
        }
        
        if (cnt >= (di->start + di->delay))
            break;
    }

    rc--;

    async_end;
}

