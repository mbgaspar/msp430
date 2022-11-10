/*
 * motores.c
 *
 *  Created on: 8 de nov de 2022
 *      Author: marce
 *
 *
 *      - Definição do TB3CCR0 foi feita apartir da frequência do clock que é 16MHz,
 *      como foi utilizada a função ID_3, então se divide o clock por 8 e a frequência do timer
 *      fica igual a 2MHz, sabendo que o modo do timer é up/down e que o periodo requisitado do PWM
 *      é de 1ms, o TB3CCR0 tem que ser (0,001)/(2*(2*10^6)^-1) que é igual a 1000.
 *
 *
 *       .
 *      /|\                  +  --> TB3CCR0 = 1000
 *       |                 +   +
 *       |               +       +
 *       |             +           +
 *       |           +               +
 *       |         +                   +
 *       |       +                       +
 *       |     +                           +
 *       |   +                               +
 *       | +                                   + --> TB3CCTL1  TB3CCTL2 TB3CCTL3 TB3CCTL4
 *       +----------------------------------------->
 *
 *       PWM configurado no modo 6, para todos os comparadores quando ativados:
 *
 *       .
 *      /|\
 *       |
 *       |   + + + + + + + + + + + + + + + + +
 *       |   +                               +
 *       |   +                               +
 *       |   +                               +
 *       |   +                               +
 *       +----------------------------------------->
 *
 *
 */
#include <msp430.h>
#include <stdint.h>
#include <bits.h>
#include <gpio.h>
#include "motores.h"

#define M1 BIT0
#define M1N BIT1
#define M2 BIT2
#define M2N BIT3


#ifndef __MSP430FR2355__
#error "Library no supported/validated in this device."
#endif

volatile uint8_t direcao = PARADO;


uint8_t obter_direcao(){
    return direcao;
}

void init_clock_system(void) {

    // Configure two FRAM wait state as required by the device data sheet for MCLK
    // operation at 24MHz(beyond 8MHz) _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_2 ;

    P2SEL1 |= BIT6 | BIT7;                       // P2.6~P2.7: crystal pins
    do
    {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);           // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);                   // Test oscillator fault flag

    __bis_SR_register(SCG0);                     // disable FLL
    CSCTL3 |= SELREF__XT1CLK;                    // Set XT1 as FLL reference source
    CSCTL0 = 0;                                  // clear DCO and MOD registers
    CSCTL1 = DCORSEL_5;                          // Set DCO = 24MHz
    CSCTL2 = FLLD_0 + 489;                       // DCOCLKDIV = 327358*731 / 1
    __delay_cycles(3);
    __bic_SR_register(SCG0);                     // enable FLL
    while(CSCTL5 & (FLLUNLOCK0 | FLLUNLOCK1));   // FLL locked

    /* CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK;
     * set XT1 (~32768Hz) as ACLK source, ACLK = 32768Hz
     * default DCOCLKDIV as MCLK and SMCLK source
     - Selects the ACLK source.
     * 00b = XT1CLK with divider (must be no more than 40 kHz)
     * 01b = REFO (internal 32-kHz clock source)
     * 10b = VLO (internal 10-kHz clock source) (1)   */
    CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK;
}


void config_timerB_3_as_pwm(){
    /* Configura comparador 1 do timer B:
     * CM_3: captura de borda de subida e descida
     * CCIS_0: entrada A
     * CCIE: ativa IRQ
     * CAP: modo captura
     * SCS: captura síncrona
     */



    /* Configura timerB1:
     * - TBSSEL_2: SMCLK como clock source
     * - MC_2: modo de contagem contínua
     * - TBCLR: limpa registrador de contagem
     * - ID_3: divide a Frequencia de clock por 8
     */
    TB3CTL |= TBSSEL_2 | MC_3 | ID_3;

    TB3CCR0 = 1000;

}

//Motores M1 e M2 Ligados para ir para frente
void frente (){


    TB3CCTL1 = OUTMOD_6;
    TB3CCTL2 = OUTMOD_0;
    TB3CCTL3 = OUTMOD_6;
    TB3CCTL4 = OUTMOD_0;

    TB3CCR1 = 0;
    TB3CCR3 = 0;

}

//Motores M1N e M2N Ligados para ir para trás
void back (){


    TB3CCTL1 = OUTMOD_0;
    TB3CCTL2 = OUTMOD_6;
    TB3CCTL3 = OUTMOD_0;
    TB3CCTL4 = OUTMOD_6;

    TB3CCR2 = 0;
    TB3CCR4 = 0;
}

//Motores M1N e M2 Ligados para ir para direita
void right (){


    TB3CCTL1 = OUTMOD_0;
    TB3CCTL2 = OUTMOD_6;
    TB3CCTL3 = OUTMOD_6;
    TB3CCTL4 = OUTMOD_0;

    TB3CCR2 = 0;
    TB3CCR3 = 0;

}

//Motores M1 e M2N Ligados para ir para esquerda
void left (){


    TB3CCTL1 = OUTMOD_6;
    TB3CCTL2 = OUTMOD_0;
    TB3CCTL3 = OUTMOD_0;
    TB3CCTL4 = OUTMOD_6;

    TB3CCR1 = 0;
    TB3CCR4 = 0;

}

//todos os motores parados para parar o carrinho
void stop (){
    TB3CCTL1 = OUTMOD_0;
    TB3CCTL2 = OUTMOD_0;
    TB3CCTL3 = OUTMOD_0;
    TB3CCTL4 = OUTMOD_0;

}

void init_motores()
{
    /* Ligação físicas do timer nas portas */

    P6DIR = M1 | M1N | M2 | M2N;


    // Função alternativa: ligação dos pinos no temporizador

    P6SEL0 = M1 | M1N | M2 | M2N;

}












