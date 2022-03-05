#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "oled.h" 
#include "uart.h"
#include "led.h"
//数据接收流程位于usart.c串口中断中，可传输入屏幕长度的数据也可以不止4个数

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
