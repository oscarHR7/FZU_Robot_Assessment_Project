#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "oled.h"
extern  unsigned char gImage_3[294];


int main(void)
{
	float time = 0.1;
  u8 number = 0;
	char charlist[5] ={0}; 
	uart_init(115200);		//³õÊ¼»¯´®¿Ú
//	clock_init();
		delay_init();	    	 
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	 OLED_Init();			
	while(1)
	{		
		OLED_Clear();
			if(number<9)
				{
						
						
						sprintf(charlist,"%1.1f ",time);
						OLED_ShowString(36,2,(u8 *)charlist,2);
						delay_ms(1000);
						number++;
						time += 0.1;
				}
			else
					{
							
							OLED_DrawBMP(40,0,88,8, gImage_3);
							sprintf(charlist,"%1.1f",time);
							OLED_ShowString(50,90,(u8 *)charlist,2);
							delay_ms(2000);
							time = 0.1;
							number = 0;
					}
	}		
		
}


