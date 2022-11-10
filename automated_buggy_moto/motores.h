/*
 * motores.h
 *
 *  Created on: 8 de nov de 2022
 *      Author: marce
 */

#ifndef MOTORES_H_
#define MOTORES_H_

enum {PARADO, FRENTE, TRAS, DIR, ESQ}; //variveis para obter a direção
uint8_t obter_direcao();

void config_timerB_3_as_pwm();
void init_motores();
void init_clock_system(void);

void frente ();
void back ();
void right ();
void left ();
void stop ();

#endif /* MOTORES_H_ */
