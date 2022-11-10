#include <msp430.h>
#include <stdint.h>

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

    /* Selecionar DCORSEL_xyz de acordo com a máxima frequência da CPU */
    CSCTL1 = DCORSEL_7;                          // Set DCO = 24MHz

    /* Ajustar o multiplicador (ex. 731) para a frequência desejada *
     *
     * F = 32768*731 / 1  ---->   (32768 * n) / FLLD_x
     */
    CSCTL2 = FLLD_0 + 731;                       // DCOCLKDIV = 32768*731 / 1
    __delay_cycles(3);
    __bic_SR_register(SCG0);                     // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));   // FLL locked

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

    /* Estamos usando TA1CCR0 para contagem máxima
     * que permite controle preciso sobre o período
     * é possível usar o overflow */

    /* Configuração dos comparadores como PWM:
     *
     * TA1CCR0: Timer1_A Capture/Compare 0: período do PWM
     *
     * OUTMOD_2: PWM output mode: 2 - PWM toggle/reset
     *
     * TA1CCR1 PWM duty cycle: TA1CCR1 / TA1CCR0 *
     * TA2CCR1 PWM duty cycle: TA2CCR1 / TA1CCR0 */

    TB3CCR0 = 3000-1; //frequencia de 1khz

    TB3CCTL1 = OUTMOD_2;
    TB3CCTL2 = OUTMOD_2;

    TB3CCR1 = 1000;
    TB3CCR2 = 500;

    /*      .
    *      /|\                  +                < -Comparador 0: (máximo da contagem) -> período do PWM
    *       |                 +   +
    *       |               +       +
    *       |-------------+---------- +          <--  Comparadores 1 e 2: razão cíclica
    *       |           +  |         | +
    *       |         +    |         |   +
    *       |       +      |         |     +
    *       |     +        |         |       +
    *       |   +          |         |         +
    *       | +            |         |           +
    * Timer +--------------|---- ----|-------------->
    *       |              |
    *       |
    *
    *       |--------------+         |--------------
    * Saída |              |         |
    *       +---------------++++++++++------------->
    */

    /* TASSEL_2 -> Timer A clock source select: 2 - SMCLK
     * MC_1     -> Timer A mode control: 1 - Up to CCR0
     * ID_3     ->  Timer A input divider: 3 - /8
     *
     * Configuração da fonte do clock do timer 1 */
    TB3CTL = TBSSEL_2 | MC_3 | ID_3;
}


void main(void)
{


    /* Para o watchdog timer
     * Necessário para código em depuração */
    WDTCTL = WDTPW | WDTHOLD;

#if defined (__MSP430FR2355__)
    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    init_clock_system();

    config_timerB_3_as_pwm();


    /* Portas não utilizadas */
        P1DIR = 0xff;
        P1OUT = 0;
        P3DIR = 0xff;
        P3OUT = 0;

    P6DIR = BIT0 | BIT1 | BIT2 | BIT3;

    P6SEL0 = BIT0 | BIT1 | BIT2 | BIT3;

    while (1)
    {




    }
}
