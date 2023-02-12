
#include <avr/io.h>
#include "async_delay.h"
#define F_CPU 4e6

static const unsigned long long F_RTC = 2048;


void delay_rtc_init() {
    RTC.CLKSEL = RTC_CLKSEL_OSC32K_gc;

    while (RTC.STATUS & RTC_CTRLABUSY_bm);
    RTC.CTRLA = RTC_RUNSTDBY_bm | RTC_PRESCALER_DIV16_gc | RTC_RTCEN_bm;
}


inline async delay(delay_info *di) {
    async_begin(di);
    static unsigned int rc = 0;


    await_while(RTC.STATUS & RTC_CNTBUSY_bm);

    if (rc == 0) {
        RTC.CNT = 0;
    }


    di->start = RTC.CNT;
    di->delay = ((di->ms * F_RTC) / 1000);


    di->overflow = false;
    if ((di->start + di->delay) < di->start) {
        di->overflow = true;
    }
    
    rc++;


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

