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
	RCC_DeInit();//RCC�Ĵ���ȡ����ʼ������
	RCC_HSEConfig(RCC_HSE_ON);//�����ⲿ����ʱ��
	RCC_WaitForHSEStartUp();//�ȴ��ⲿ��������
	FLASH_SetLatency(FLASH_Latency_2);//������ʱ2����
	FLASH_HalfCycleAccessCmd(FLASH_HalfCycleAccess_Disable);//�����ڷ���ʧ��
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);//Ԥȡָ����ʹ��
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);//�ⲿ����ʱ��ΪPLLʱ������Դ��9��Ƶ
	RCC_PLLCmd(ENABLE);//ʹ��PLL
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//PLL��Ϊϵͳʱ��
	RCC_HCLKConfig(RCC_SYSCLK_Div1);//AHB ʱ�� = ϵͳʱ��
	RCC_PCLK1Config(RCC_HCLK_Div2);//APB1 ʱ�� = HCLK / 2,��AHBʱ�ӵ�һ��
	RCC_PCLK2Config(RCC_HCLK_Div1);//APB2 ʱ�� = HCLK����AHBʱ��
	//RCC_ADCCLKConfig(RCC_PCLK2_Div6);//ADC ʱ�� = PCLK / 6����APB2ʱ�ӣ�72/6=12M
	RCC_LSEConfig(RCC_LSE_OFF);//�ر��ⲿ����ʱ��
}
/******************************************************************************/
void GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);//ʹ��GPIOA,GPIOB,GPIOC,AFIO;
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;        //��LED
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�����������	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//�ٶ�
	GPIO_Init(GPIOA, &GPIO_InitStructure);           //��ѡ�йܽų�ʼ��
	GPIO_SetBits(GPIOA,GPIO_Pin_3);                  //�ϵ����LED
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
				TCS34725_GetRawData(&rgb);  //�����Σ�ʵ�ʲ���ʱ���ֶ�������ɫ������һ�ε���ɫ
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

