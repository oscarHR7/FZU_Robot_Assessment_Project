#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "oled.h" 
#include "uart.h"
#include "led.h"
//���ݽ�������λ��usart.c�����ж��У��ɴ�������Ļ���ȵ�����Ҳ���Բ�ֹ4����

 int main(void)
 {		
	 u8 i = 1;
					delay_init();	    	 
					NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	 
					USART1_Init(9600);	 
					OLED_Init();
					OLED_Clear();	
					LED_Init();	 
			while(1)
				{
					
					LED0 = 0;
				}
 }
