#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
  //TIM3 PWM初始化 arr重装载值 psc预分频系数
		GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef     TIM_OCInitStrue;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
    
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//使能TIM3和相关GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//使能AFIO时钟(定时器3通道3需要重映射到BP5引脚)
    
//    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE); //映射，重映射只用于64、100、144脚单片机
   //当没有重映射时，TIM3的四个通道CH1，CH2，CH3，CH4分别对应PA6，PA7,PB0,PB1
   //当部分重映射时，TIM3的四个通道CH1，CH2，CH3，CH4分别对应PB4，PB5,PB0,PB1 (GPIO_PartialRemap_TIM3)
   //当完全重映射时，TIM3的四个通道CH1，CH2，CH3，CH4分别对应PC6，PC7,PC8,PC9 (GPIO_FullRemap_TIM3) 
	      
    TIM_TimeBaseInitStrue.TIM_Period=7200 - 1;    //设置自动重装载值
    TIM_TimeBaseInitStrue.TIM_Prescaler = 0;        //预分频系数 1k 72 10k 36  20k
    TIM_TimeBaseInitStrue.TIM_CounterMode = TIM_CounterMode_Up;    //计数器向上溢出
    TIM_TimeBaseInitStrue.TIM_ClockDivision = TIM_CKD_DIV1;        //时钟的分频因子，起到了一点点的延时作用，一般设为TIM_CKD_DIV1
		TIM_TimeBaseInitStrue.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue);        //TIM3初始化设置(设置PWM的周期)
    
		TIM_OCStructInit(&TIM_OCInitStrue);
    TIM_OCInitStrue.TIM_OCMode=TIM_OCMode_PWM1;        // PWM模式1:CNT < CCR时输出有效电平
    TIM_OCInitStrue.TIM_OCPolarity=TIM_OCPolarity_High;// 设置极性-有效电平为：高电平
    TIM_OCInitStrue.TIM_OutputState=TIM_OutputState_Enable;// 输出使能
		TIM_OCInitStrue.TIM_Pulse = 0;
		
		TIM_OC3Init(TIM3,&TIM_OCInitStrue); 
		TIM_OC4Init(TIM3,&TIM_OCInitStrue); 
		
         //使能预装载寄存器
		
    TIM_Cmd(TIM3,ENABLE);        //使能TIM3
    
}



void PWM_SetCompare3(uint16_t Compare3)
{
	TIM_SetCompare3(TIM3, Compare3);
}

void PWM_SetCompare4(uint16_t Compare4)
{
	TIM_SetCompare4(TIM3, Compare4);
}

void PWM_Limiting(int *motor1, int *motor2)
{
	int Amplitude = 6900;
	if(*motor1 < -Amplitude) *motor1 = -Amplitude;
	if(*motor1 > Amplitude) *motor1 = Amplitude;
	if(*motor2 < -Amplitude) *motor2 = -Amplitude;
	if(*motor2 > Amplitude) *motor2 = Amplitude;
}






