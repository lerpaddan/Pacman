
/* led.c */

#include <pic32mx.h>
#include "peripherals.h"


/* LEDs are mapped to PORTE
 */
void led_init(void)
{
        /* Set all bits in PORTE to OUTPUT */
        TRISECLR = 0xff;
        /* Initialize all LEDs to be OFF */
        PORTECLR = 0xff;
}


void led_clear(void)
{
        PORTECLR = 0xff;
}


void led_set(uint8_t code)
{
        PORTECLR = ~code;
        PORTESET = code;
}

