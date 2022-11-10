/*
 *                  MSP430FR2355
 *               -----------------
 *              |              RX |
 *              |              TX |
 *              | P1.0    P2.1    |
 *              |         P6.0    | --> M1
 *              |         P6.1    | --> M1N
 *              |         P6.2    | --> M2
 *              |         P6.3    | --> M2N
 *              |         P2.4    |
 *
 */



#include <msp430.h>
#include <stdint.h>


#include <bits.h>
#include <gpio.h>
#include <motores.h>


int main(void)
{
#ifndef __MSP430FR2355__
#error "Clock system not supported for this device"
#endif

#if defined (__MSP430FR2355__)
    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;

    init_clock_system(); //inicializa o clock
    config_timerB_3_as_pwm(); //configura o timer para o PWM
    init_motores(); //inicializa as portas dos motores

    __bis_SR_register(GIE);

    while(1){

        right (); //ir frente
        _delay_cycles(2000000);

        back(); //ir traz
        _delay_cycles(2000000);

        left(); // ir esquerda
        _delay_cycles(2000000);

        right(); //ir direita
        _delay_cycles(2000000);

        stop(); // parar
        _delay_cycles(2000000);


        __bis_SR_register(LPM4_bits);
    }



}
