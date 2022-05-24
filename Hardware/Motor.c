#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Motor.h"
int Dead_Zone = 185;
int control_turn = 64;
void Motor_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStrue;
		
	 GPIO_InitStrue.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_InitStrue.GPIO_Pin = GPIO_Pin_9 |GPIO_Pin_10 |GPIO_Pin_11|GPIO_Pin_12;
   GPIO_InitStrue.GPIO_Speed = 	GPIO_Speed_50MHz;	
	 GPIO_Init(GPIOA, &GPIO_InitStrue);
	
	 GPIO_ResetBits(GPIOA,GPIO_Pin_9);
	 GPIO_ResetBits(GPIOA,GPIO_Pin_10);
	 GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	 GPIO_ResetBits(GPIOA,GPIO_Pin_12);
}

void Motor_SetSpeed(int motor1, int motor2)
{
	if(motor1>0) AIN1 = 1, AIN2 = 0;
	else 
	{
		AIN1 = 0, AIN2 = 1;
		motor1 = - motor1;
	}
	int speed1 = Dead_Zone + motor1;
	PWM_SetCompare3(speed1);
	
	if(motor2>0) BIN1 = 1, BIN2 = 0;
	else
	{		
		BIN1 = 0, BIN2 = 1;
		motor2 = -motor2;
	}
	int speed2 = Dead_Zone + motor2;
	PWM_SetCompare4(speed2);
}
void go_ahead(void)
{
	AIN1 = 1, AIN2 = 0;
	BIN1 = 1, BIN2 = 0;  
	PWM_SetCompare3(4600);
	PWM_SetCompare4(4600);
}
