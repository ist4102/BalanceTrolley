#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
  //TIM3 PWM��ʼ�� arr��װ��ֵ pscԤ��Ƶϵ��
		GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef     TIM_OCInitStrue;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
    
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//ʹ��TIM3�����GPIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//ʹ��AFIOʱ��(��ʱ��3ͨ��3��Ҫ��ӳ�䵽BP5����)
    
//    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE); //ӳ�䣬��ӳ��ֻ����64��100��144�ŵ�Ƭ��
   //��û����ӳ��ʱ��TIM3���ĸ�ͨ��CH1��CH2��CH3��CH4�ֱ��ӦPA6��PA7,PB0,PB1
   //��������ӳ��ʱ��TIM3���ĸ�ͨ��CH1��CH2��CH3��CH4�ֱ��ӦPB4��PB5,PB0,PB1 (GPIO_PartialRemap_TIM3)
   //����ȫ��ӳ��ʱ��TIM3���ĸ�ͨ��CH1��CH2��CH3��CH4�ֱ��ӦPC6��PC7,PC8,PC9 (GPIO_FullRemap_TIM3) 
	      
    TIM_TimeBaseInitStrue.TIM_Period=7200 - 1;    //�����Զ���װ��ֵ
    TIM_TimeBaseInitStrue.TIM_Prescaler = 0;        //Ԥ��Ƶϵ�� 1k 72 10k 36  20k
    TIM_TimeBaseInitStrue.TIM_CounterMode = TIM_CounterMode_Up;    //�������������
    TIM_TimeBaseInitStrue.TIM_ClockDivision = TIM_CKD_DIV1;        //ʱ�ӵķ�Ƶ���ӣ�����һ������ʱ���ã�һ����ΪTIM_CKD_DIV1
		TIM_TimeBaseInitStrue.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue);        //TIM3��ʼ������(����PWM������)
    
		TIM_OCStructInit(&TIM_OCInitStrue);
    TIM_OCInitStrue.TIM_OCMode=TIM_OCMode_PWM1;        // PWMģʽ1:CNT < CCRʱ�����Ч��ƽ
    TIM_OCInitStrue.TIM_OCPolarity=TIM_OCPolarity_High;// ���ü���-��Ч��ƽΪ���ߵ�ƽ
    TIM_OCInitStrue.TIM_OutputState=TIM_OutputState_Enable;// ���ʹ��
		TIM_OCInitStrue.TIM_Pulse = 0;
		
		TIM_OC3Init(TIM3,&TIM_OCInitStrue); 
		TIM_OC4Init(TIM3,&TIM_OCInitStrue); 
		
         //ʹ��Ԥװ�ؼĴ���
		
    TIM_Cmd(TIM3,ENABLE);        //ʹ��TIM3
    
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






