#include "stm32f10x.h"                  // Device header

 void My_USART2_Init(void)  
{  
    GPIO_InitTypeDef GPIO_InitStrue;  
    USART_InitTypeDef USART_InitStrue;  
    NVIC_InitTypeDef NVIC_InitStrue;  
      
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能串口的RCC时钟
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;  
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_2;        //Tx
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;  
    GPIO_Init(GPIOA,&GPIO_InitStrue);  
      
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;  
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_3;     //Rx
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;  
    GPIO_Init(GPIOA,&GPIO_InitStrue);  
      
    USART_InitStrue.USART_BaudRate=9600;  
    USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;  
    USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;  
    USART_InitStrue.USART_Parity=USART_Parity_No;  
    USART_InitStrue.USART_StopBits=USART_StopBits_1;  
    USART_InitStrue.USART_WordLength=USART_WordLength_8b;  
      
    USART_Init(USART2,&USART_InitStrue);
      
    USART_Cmd(USART2,ENABLE); 
      
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);  
      
    NVIC_InitStrue.NVIC_IRQChannel=USART2_IRQn;  
    NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;  
    NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1;  
    NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;  
    NVIC_Init(&NVIC_InitStrue);  
      
}  
//  
//void USART2_IRQHandler(void)  
//{  
//     
//		if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)  
//		{
//			res= USART_ReceiveData(USART2); 
//			
//		}  
//	
//}
