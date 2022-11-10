#include <msp430.h>
#include <stdint.h>

#include "motor.h"
#include "bits.h"


void motor_init()
{
	TB3CCR0 = 100; // 8mhz/8k = 1khz

	TB3CCTL1 = OUTMOD_7;
	TB3CCR1 = 0;
	TB3CCTL2 = OUTMOD_7;
	TB3CCR2 = 0;

	//TA0CTL = TASSEL_2 | MC_1 | ID_3;  //8MHZ/1000/8 = 1ms

	TB3CTL = TBSSEL_1 | MC_1 ;  //32768hz/100 = 3.051ms

	MOTOR_DIRECTION_DIR |= (MOTOR_DIRECTION_LEFT | MOTOR_DIRECTION_RIGHT);
	MOTOR_PWM_PORT_DIR = (MOTOR_PWM_LEFT | MOTOR_PWM_RIGHT);

}

void set_motor_pwm(uint16_t pwm)
{
	TB3CCR1 = pwm;
	TB3CCR2 = pwm;
}

void motor_move(uint8_t direction)
{
	switch (direction)
	{

	case FOWARD:
		MOTOR_DIRECTION_OUT |= (MOTOR_DIRECTION_LEFT | MOTOR_DIRECTION_RIGHT);
		MOTOR_PWM_PORT_SEL = (MOTOR_PWM_LEFT | MOTOR_PWM_RIGHT);
		break;

	case RIGHT:
		MOTOR_DIRECTION_OUT |= MOTOR_DIRECTION_RIGHT;
		MOTOR_DIRECTION_OUT &= ~MOTOR_DIRECTION_LEFT;
		MOTOR_PWM_PORT_SEL = (MOTOR_PWM_LEFT | MOTOR_PWM_RIGHT);
		break;

	case BACKWARD:
		MOTOR_DIRECTION_OUT &= ~(MOTOR_DIRECTION_LEFT | MOTOR_DIRECTION_RIGHT);
		MOTOR_PWM_PORT_SEL = (MOTOR_PWM_LEFT | MOTOR_PWM_RIGHT);
		break;

	case LEFT:
		MOTOR_DIRECTION_OUT |= MOTOR_DIRECTION_LEFT;
		MOTOR_DIRECTION_OUT &= ~MOTOR_DIRECTION_RIGHT;
		MOTOR_PWM_PORT_SEL = (MOTOR_PWM_LEFT | MOTOR_PWM_RIGHT);
		break;



	default:
		break;
	}

}
