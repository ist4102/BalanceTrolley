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
	 Bias=pitch - ZHONGZHI;  	//===求出平衡的角度中值 和机械相关
	 balance = Balance_Kp*Bias + acc_x*Balance_Kd;   //===计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数 
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
	  //=============遥控左右旋转部分=======================//
  	if(1==Flag_Left||1==Flag_Right)                      //这一部分主要是根据旋转前的速度调整速度的起始速度，增加小车的适应性
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
		if(1==Flag_Left)	           Turn_Target -= Turn_Convert;        //===接收转向遥控数据
		else if(1==Flag_Right)	     Turn_Target += Turn_Convert;        //===接收转向遥控数据
		else Turn_Target=0;                                            //===接收转向遥控数据
    if(Turn_Target > Turn_Amplitude)  Turn_Target=Turn_Amplitude;    //===转向速度限幅
	  if(Turn_Target < -Turn_Amplitude) Turn_Target=-Turn_Amplitude;   //===转向速度限幅
		if(Flag_Qian==1||Flag_Hou==1)  Kd=0.8;                         //===接收转向遥控数据直立行走的时候增加陀螺仪就纠正    
		else Kd=0.7;                                   
  	//=============转向PD控制器=======================//
		Turn=Turn_Target*Kp+gyro*Kd;                 //===结合Z轴陀螺仪进行PD控制
	  return Turn;
}











