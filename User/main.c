#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "OLED.h"
#include "Motor.h"
#include "PWM.h"
#include "i2c.h"
#include "encoder.h"
#include "Timer.h"
#include "control.h"
#include "MPU6050.h"

extern short gyro[3], accel[3], sensors;
extern float Pitch,Roll,Yaw; 							// ¸©Ñö½Ç ·­¹ö½Ç Æ«º½½Ç 
int Pwm1, Pwm2;
float Angle_Balance,Gyro_Balance,Gyro_Turn; 
int k = -7;
char str[8];  
char* fts(float f, char* str); //¸¡µãÊı×ª×Ö·û´®
int Balance_Pwm,Velocity_Pwm,Turn_Pwm;
int Encoder_A,Encoder_B;
uint8_t res;
 
int Flag_Left = 1,Flag_Right,Flag_Qian,Flag_Hou;
int main(void)
{
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	  delay_ms(500);
	  OLED_Init();
	  Motor_Init();
	  
	  i2cInit();
	  PWM_Init();
	 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	  Encoder_Init_TIM2();
    Encoder_Init_TIM4();
		
		MPU6050_Init();
  	OLED_ShowString(2, 1, "Pitch:");
//	  OLED_ShowString(3, 1, "ROll:");
// 	  OLED_ShowString(4, 1,"Yaw:");
		
 
	   TIM1_Int_Init();
	 //Motor_SetSpeed(185, 185);
     while(1)
		 {
			
		 }
}


void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		 TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
		 MPU6050_Pose();	
		 OLED_ShowString(2,7,fts(Pitch,str));
		// OLED_ShowString(3,7,fts(Roll,str));
		// OLED_ShowString(4,7,fts(Yaw,str));
		 Encoder_A =  Read_Encoder(2);
		 Encoder_B =  Read_Encoder(4);
		 Gyro_Balance = gyro[0];
		 //Velocity_Pwm = Vertical_speed_PI(Encoder_A, Encoder_B, Pitch, 0);
		 Balance_Pwm = balance(Pitch, Gyro_Balance);
//		 Turn_Pwm = Vertical_turn_PD(Encoder_A,Encoder_B, gyro[2]);
		 Pwm1 = Balance_Pwm + Velocity_Pwm + Turn_Pwm;
		 Pwm2 = Balance_Pwm  + Velocity_Pwm  - Turn_Pwm; 
		 PWM_Limiting(&Pwm1, &Pwm2);
	
	
		 //OLED_ShowNum(3, 7, gyro[0], 5);
	 //OLED_ShowNum(4, 7, Encoder_B, 5);
		 Motor_SetSpeed(Pwm1, Pwm2);	
	}
}
  
void USART2_IRQHandler(void)  
{  
     
		if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)  
		{
			res = USART_ReceiveData(USART2); 
			
		}  
	
}

char* fts(float f, char* str)
{
	if(f>=0) str[0] = '+';
	else 
	{
		str[0] = '-';
		f = -f;
	}
	char temp[10] = {'0', '1', '2', '3','4', '5', '6', '7', '8', '9'};
	int i = 2, j = 4;
	int zheng = f/1;
	for(int n = 0;n < i;n++)
	{
		if(zheng == 0)
			str[i - n] = temp[0];
			
		else
			str[i - n] = temp[zheng % 10];
		zheng/=10;
	}
	str[3] = '.';
	int xiao = 0;
	for(int n = j;n < 6;n++)
	{
		xiao = f*10;
		str[n] = temp[xiao % 10];
		f*=10; 
	}
	str[7] = '\0';
	return str;

}
