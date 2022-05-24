#ifndef __CONTROL_H
#define __CONTROL_H

int myabs(int a);
int balance(float pitch,float acc_x);
int Vertical_speed_PI(short encoder_left, short encoder_right, float angle, float Movement);
int Vertical_turn_PD(short encoder_left, short encoder_right, short gyro);
uint8_t Turn_off(float angle);
#endif
