#include <msp430.h>
#include <stdint.h>
#include "simple_display_mux.h"

#define D1 = BIT0
#define D2 = BIT1

/* Tabela de conversão em flash: Anodo comum */
#ifdef COM_ANODO
const uint8_t convTable[] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02,
        0x78, 0x00, 0x18, 0x08, 0x03, 0x46, 0x21, 0x06, 0x0E};
#endif


void display_mux_init() {

    /* Configuração de portas */
    DISPLAYS_DATA_PORT_DIR = 0xff;
    DISPLAYS_DATA_PORT_OUT = 0;

    DISPLAYS_MUX_PORT_DIR |= D1 | D2 //colocando D1 e p2 como saida.
}


/* Função para escrever em múltiplos displays usando */
void display_mux_write(uint8_t data){

	/* Desliga todos os displays */
    CLR_BIT(0, D1);
    CLR_BIT(0, D2);

	/* Escreve valor convertido do dígito 1 no GPIO */


	/* Liga display 1 */
    SET_BIT(1, D1);


	/* Mantém um tempo ligado:  */
	_delay_cycles(10000);

	/* Desliga display 1 */
	CLR_BIT(0, D1);

	/* Escreve valor convertido do dígito 2 no GPIO */


	/* Liga display 2 */
	SET_BIT(1, D2);

	/* Mantém um tempo ligado */
	_delay_cycles(10000);
}

/* Função para escrever em múltiplos displays usando for */

void display_mux_write_for(uint8_t data){
    int8_t n = NUMBER_DISPLAYS;

    for (n=NUMBER_DISPLAYS; n > 0; n--){
        /* Desliga todos os displays */
        CLR_BIT(0, D1);
        CLR_BIT(0, D2);

        /* Escreve valor convertido do dígito n no GPIO */

        /* Liga display n */

        /* Mantém um tempo ligado:  */
        _delay_cycles(10000);

    }
}
