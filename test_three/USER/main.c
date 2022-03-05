#include "stm32f10x.h"
#include "config.h"
#include "sys.h"
#include "usart.h"
#include "tcs34725.h"
#include "oled.h"
#include "delay.h"
extern COLOR_RGBC rgb;
extern COLOR_HSL  hsl;
extern u8 TCS34725_GetRawData(COLOR_RGBC *rgbc);
extern u8 TCS34725_Init(void);

/******************************************************************************/
void com2_order(void);
/******************************************************************************/
void delay_s(u32 i)
{
	while(i--);
}
/******************************************************************************/
void RCC_Configuration(void)
{
	RCC_DeInit();//RCC寄存器取消初始化设置
	RCC_HSEConfig(RCC_HSE_ON);//开启外部高速时钟
	RCC_WaitForHSEStartUp();//等待外部晶振启动
	FLASH_SetLatency(FLASH_Latency_2);//代码延时2周期
	FLASH_HalfCycleAccessCmd(FLASH_HalfCycleAccess_Disable);//半周期访问失能
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);//预取指缓存使能
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);//外部高速时钟为PLL时钟输入源，9倍频
	RCC_PLLCmd(ENABLE);//使能PLL
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//PLL作为系统时钟
	RCC_HCLKConfig(RCC_SYSCLK_Div1);//AHB 时钟 = 系统时钟
	RCC_PCLK1Config(RCC_HCLK_Div2);//APB1 时钟 = HCLK / 2,即AHB时钟的一半
	RCC_PCLK2Config(RCC_HCLK_Div1);//APB2 时钟 = HCLK，即AHB时钟
	//RCC_ADCCLKConfig(RCC_PCLK2_Div6);//ADC 时钟 = PCLK / 6，即APB2时钟，72/6=12M
	RCC_LSEConfig(RCC_LSE_OFF);//关闭外部低速时钟
}
/******************************************************************************/
void GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);//使能GPIOA,GPIOB,GPIOC,AFIO;
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;        //是LED
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //复用推挽输出	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);           //对选中管脚初始化
	GPIO_SetBits(GPIOA,GPIO_Pin_3);                  //上电点亮LED
}
/******************************************************************************/
/******************************************************************************/
int main(void)
{
	
	RCC_Configuration();
	GPIO_Config();
	uart_init(115200);
	delay_s(0x1fffff);
	delay_init();
//	TCS34725_Init();
	if(TCS34725_Init())
	{printf("Initial OK!\r\n");}
	else
	{
		printf("Initial Error!\r\n");
	}
	delay_ms(1000);
	OLED_Init();
	char charlist[5] ={0}; 
	while(1)
	{	
				u8 max = 0;
				u8 R = 0, G = 0,B = 0;
				u16 C = 0;
				TCS34725_GetRawData(&rgb);  //读两次，实际测试时发现读到的颜色总是上一次的颜色
				RGBtoHSL(&rgb,&hsl);			
				OLED_Clear();
				OLED_ShowString(0,0,"R=",2);
				sprintf(charlist,"%d ",rgb.r);
				OLED_ShowString(20,0,(u8 *)charlist,2);
				OLED_ShowString(0,2,"G=",2);
				sprintf(charlist,"%d ",rgb.g);
				OLED_ShowString(20,2,(u8 *)charlist,2);
				OLED_ShowString(0,4,"B=",2);
				sprintf(charlist,"%d ",rgb.b);
				OLED_ShowString(20,4,(u8 *)charlist,2);
				OLED_ShowString(0,6,"C=",2);
				sprintf(charlist,"%d ",rgb.c);
				OLED_ShowString(20,6,(u8 *)charlist,2);
				max = max3v(rgb.r,rgb.g,rgb.b);
				R = rgb.r;
				G = rgb.g;
				B = rgb.b;
				C =	rgb.c;
				if(C<45)
				{
					OLED_ShowCHinese(80,10,3);
				}
				else if(C>500)
				{
					OLED_ShowCHinese(80,10,4);
				}
				else if(R==max)	
				{
					OLED_ShowCHinese(80,10,0);
				}
				else if(G==max)
				{
					OLED_ShowCHinese(80,10,1);
				}
				else 
				{
					OLED_ShowCHinese(80,10,2);
				}
				
	}
				

}

