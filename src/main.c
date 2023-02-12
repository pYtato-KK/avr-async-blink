#define F_CPU 4e6

#include <avr/io.h>
#include <util/delay.h>

#include "async_delay.h"


typedef unsigned char pin_t;


typedef struct {
    async_state;
    PORT_t *port;
    pin_t pin;
    unsigned period;
    delay_info dpt;
} blink_state;


async blink(blink_state *pt) {
    async_begin(pt);

    while (1) {
        async_init(&(pt->dpt));
        pt->dpt.ms = pt->period;

        await(delay(&(pt->dpt)));

        pt->port->OUTTGL = 1 << pt->pin;
    }

    async_end;
}


blink_state blink_init(PORT_t *port, pin_t pin, unsigned period) {
    blink_state bs;

    async_init(&bs);

    bs.port = port;
    bs.pin = pin;
    bs.period = period;

    return bs;
}


void main() {
    blink_state pt1 = blink_init(&PORTA, 4, 314);
    blink_state pt2 = blink_init(&PORTA, 5, 159);
    blink_state pt3 = blink_init(&PORTA, 6, 265);


    PORTA.DIRSET = PIN4_bm | PIN5_bm | PIN6_bm;
    delay_rtc_init();


    while (1) {
        blink(&pt1);
        blink(&pt2);
        blink(&pt3);
    }
}

