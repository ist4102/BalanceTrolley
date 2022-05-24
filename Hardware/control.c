#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "OLED.h"
float ZHONGZHI = 4.5;
float Balance_Kp = 400, Balance_Kd = 0.1;
float Velocity_Kp = 50, Veclocity_Ki= 0.25;
float Turn_Kp = 18, Turn_Kd = 0.18;
extern int Flag_Left,Flag_Right,Flag_Qian,Flag_Hou;
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
int balance(float pitch,float acc_x)
	
{  
	
   float Bias;
	 int balance;
	 Bias=pitch - ZHONGZHI;  	//===���ƽ��ĽǶ���ֵ �ͻ�е���
	 balance = Balance_Kp*Bias + acc_x*Balance_Kd;   //===����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	 return balance;
}

uint8_t Turn_off(float angle)
{
	uint8_t temp;
	if(angle < -40 || angle > 40)
	{
		temp = 1;
		AIN1 = 0;
		AIN2 = 0;
		BIN1 = 0;
		BIN2 = 0;	
	}
	else 
		temp = 0;
	return temp;
}


int Vertical_speed_PI(short encoder_left, short encoder_right, float angle, float Movement)
{
	
	static float Velocity, Encoder_Least, Encoder;
	static float Encoder_integral;
	
	Encoder_Least = (encoder_left + encoder_right) - 0;
	
	Encoder *= 0.85f;
	Encoder += Encoder_Least * 0.15f;
	
	Encoder_integral += Encoder;
	Encoder_integral = Encoder_integral - Movement;
	
	if(Encoder_integral > 5000)  Encoder_integral = 5000;
	if(Encoder_integral < -5000)  Encoder_integral = -5000;
	
	Velocity = Encoder * Velocity_Kp + Encoder_integral * Veclocity_Ki;
	
	if(Turn_off(angle)) Encoder_integral = 0;
	return Velocity;
}
//int Vertical_turn_PD(uint8_t CCD, short yaw)
//{
//	float Bias;
//	float Turn;
//	Bias = CCD-64;
//	Turn = -Bias*Turn_Kp - yaw *Turn_Kd;
//	return Turn;
//}
int Vertical_turn_PD(short encoder_left, short encoder_right, short gyro)
{
	static float Turn_Target,Turn,Encoder_temp,Turn_Convert=0.7,Turn_Count,Kp=42,Kd=0;
	  float Turn_Amplitude=10;    
	  //=============ң��������ת����=======================//
  	if(1==Flag_Left||1==Flag_Right)                      //��һ������Ҫ�Ǹ�����תǰ���ٶȵ����ٶȵ���ʼ�ٶȣ�����С������Ӧ��
		{
			if(++Turn_Count==1)
			Encoder_temp=myabs(encoder_left-encoder_right);
			Turn_Convert=50/Encoder_temp;
			if(Turn_Convert<0.4)Turn_Convert=0.4;
			if(Turn_Convert>1)Turn_Convert=1;
		}	
	  else
		{
			Turn_Convert=0.7;
			Turn_Count=0;
			Encoder_temp=0;
		}		
		if(1==Flag_Left)	           Turn_Target -= Turn_Convert;        //===����ת��ң������
		else if(1==Flag_Right)	     Turn_Target += Turn_Convert;        //===����ת��ң������
		else Turn_Target=0;                                            //===����ת��ң������
    if(Turn_Target > Turn_Amplitude)  Turn_Target=Turn_Amplitude;    //===ת���ٶ��޷�
	  if(Turn_Target < -Turn_Amplitude) Turn_Target=-Turn_Amplitude;   //===ת���ٶ��޷�
		if(Flag_Qian==1||Flag_Hou==1)  Kd=0.8;                         //===����ת��ң������ֱ�����ߵ�ʱ�����������Ǿ;���    
		else Kd=0.7;                                   
  	//=============ת��PD������=======================//
		Turn=Turn_Target*Kp+gyro*Kd;                 //===���Z�������ǽ���PD����
	  return Turn;
}











