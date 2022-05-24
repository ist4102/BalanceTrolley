#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"

#define AIN1 PAout(12) 
#define AIN2 PAout(11) 
#define BIN1 PAout(9)
#define BIN2 PAout(10)

void Motor_Init(void);
void Motor_SetSpeed(int motor1, int motor2);
void go_ahead(void);


#endif
