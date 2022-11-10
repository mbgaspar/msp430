#include <msp430.h>

/* Tipos uint16_t, uint8_t, ... */
#include <stdint.h>

#include "simple_display_mux.h"

void main(void)
{
    /* Para o watchdog timer
     * Necessário para código em depuração */
    WDTCTL = WDTPW | WDTHOLD;

#if defined (__MSP430FR2355__)
    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    volatile uint16_t i;
    uint8_t x = 0;

    /* Inicializa displays */
    display_mux_init();

    while(1)
    {
        display_mux_write(x);

        /* Delay */
        for(i=10000; i>0; i--);

        /* Incrementa e limita valor de x até 0x0f */
        x++;
    }
}
