#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

#define MOTOR_PWM_PORT_SEL  P3SEL
#define MOTOR_PWM_PORT_OUT  P3OUT
#define MOTOR_PWM_PORT_DIR  P3DIR

#define MOTOR_DIRECTION_DIR P3DIR
#define MOTOR_DIRECTION_OUT P3OUT

#define MOTOR_DIRECTION_LEFT 	BIT0
#define MOTOR_DIRECTION_RIGHT 	BIT1

#define MOTOR_PWM_LEFT 	BIT2
#define MOTOR_PWM_RIGHT	BIT3

enum MOTOR_DIRECTIONS
{
	FOWARD, LEFT, RIGHT, BACKWARD,
};

void motor_init();

void set_motor_pwm(uint16_t);

void motor_move(uint8_t);

#endif /* MOTOR_H */

